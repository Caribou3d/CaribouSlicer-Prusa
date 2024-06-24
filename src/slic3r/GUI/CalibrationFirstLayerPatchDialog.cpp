#include "CalibrationFirstLayerPatchDialog.hpp"
#include "I18N.hpp"
#include "libslic3r/Model.hpp"
#include "libslic3r/Utils.hpp"
#include "GLCanvas3D.hpp"
#include "GUI.hpp"
#include "GUI_ObjectList.hpp"
#include "Plater.hpp"
#include "Tab.hpp"
#include <wx/scrolwin.h>
#include <wx/display.h>
#include <wx/file.h>
#include "wxExtensions.hpp"

#if ENABLE_SCROLLABLE
static wxSize get_screen_size(wxWindow* window)
{
    const auto idx = wxDisplay::GetFromWindow(window);
    wxDisplay display(idx != wxNOT_FOUND ? idx : 0u);
    return display.GetClientArea().GetSize();
}
#endif // ENABLE_SCROLLABLE

namespace Slic3r {
namespace GUI {

void CalibrationFirstLayerPatchDialog::create_buttons(wxStdDialogButtonSizer* buttons){
    wxString choices_dimension[] = {"20%","30%","40%","50%","60%","70%","80%","90%","100%"};
    dimension = new wxComboBox(this, wxID_ANY, wxString{ "80" }, wxDefaultPosition, wxDefaultSize, 9, choices_dimension);
    dimension->SetToolTip(_L("You can choose the size of the patch."
        " It's a simple scale, you can modify it in the right panel yourself if you prefer. It's just quicker to select it here."));
    dimension->SetSelection(4);

    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Size:")));
    buttons->AddSpacer(10);
    buttons->Add(dimension);
    buttons->AddSpacer(10);
    buttons->Add(new wxStaticText(this, wxID_ANY, "%"));
    buttons->AddSpacer(40);
    wxButton* bt = new wxButton(this, wxID_FILE1, _(L("Generate")));
    bt->Bind(wxEVT_BUTTON, &CalibrationFirstLayerPatchDialog::create_geometry_v, this);
    buttons->Add(bt);
}

void CalibrationFirstLayerPatchDialog::create_geometry() {
    Plater* plat = this->main_frame->plater();
    Model& model = plat->model();
    plat->new_project();

    const BuildVolume::Type type = plat->build_volume().type();

    bool isRect = (type == BuildVolume::Type::Rectangle);
    bool isCirc = (type == BuildVolume::Type::Circle);

    std::vector<size_t> objs_idx;

    //GLCanvas3D::set_warning_freeze(true);
    if (isRect){
        objs_idx = plat->load_files(std::vector<std::string>{
                (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer_patch"/ "square.3mf").string()
                }, true, false, false);
    } else if (isCirc){
        objs_idx = plat->load_files(std::vector<std::string>{
                (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer_patch"/ "circle.3mf").string()
                }, true, false, false);
    } else return;

    assert(objs_idx.size() == 1);
    const DynamicPrintConfig* printConfig = this->gui_app->get_tab(Preset::TYPE_PRINT)->get_config();
    const DynamicPrintConfig* printerConfig = this->gui_app->get_tab(Preset::TYPE_PRINTER)->get_config();
    const ConfigOptionPoints* bed_shape = printerConfig->option<ConfigOptionPoints>("bed_shape");

    int idx_scale = dimension->GetSelection();

    Vec2d bed_size = BoundingBoxf(bed_shape->values).size();
    Vec2d bed_min = BoundingBoxf(bed_shape->values).min;

    /// --- scale the squares according to bed dimensions ---
    float bed_dim_x = bed_size.x() - bed_min.x() - 5.0;
    float bed_dim_y = bed_size.y() - bed_min.y() - 5.0 ;

    float xScale = (idx_scale + 2.0) / 10.0 * bed_dim_x / 20.0;
    float yScale = (idx_scale + 2.0) / 10.0 * bed_dim_y / 20.0;

    double radius = unscaled<double>(plat->build_volume().circle().radius);

    if (isCirc){
        xScale = (idx_scale + 2.0) / 10.0 * 2.0 * radius * 0.95 / 20;
        yScale = xScale;
    }

    /// --- scale in z direction according to layer height and nozzle diameter ---
    const ConfigOptionFloats* nozzle_diameter_config = printerConfig->option<ConfigOptionFloats>("nozzle_diameter");
    assert(nozzle_diameter_config->values.size() > 0);
    float nozzle_diameter = nozzle_diameter_config->values[0];

    const ConfigOptionFloatOrPercent* first_layer_height_setting = printConfig->option<ConfigOptionFloatOrPercent>("first_layer_height");
    double first_layer_height = first_layer_height_setting->get_abs_value(nozzle_diameter);

    float zScale = std::max(first_layer_height, nozzle_diameter / 2.0) / 0.2;

    model.objects[objs_idx[0]]->scale(xScale, yScale, zScale);

    /// --- custom config ---
    model.objects[objs_idx[0]]->config.set_key_value("perimeters", new ConfigOptionInt(2));
    model.objects[objs_idx[0]]->config.set_key_value("top_solid_layers", new ConfigOptionInt(1));
    model.objects[objs_idx[0]]->config.set_key_value("bottom_solid_layers", new ConfigOptionInt(1));

    /// --- main config, please modify object config when possible ---
    DynamicPrintConfig new_print_config = *printConfig; //make a copy
    new_print_config.set_key_value("skirts", new ConfigOptionInt(2));
    new_print_config.set_key_value("skirt_distance", new ConfigOptionFloat(1.0));

    //update plater
    this->gui_app->get_tab(Preset::TYPE_PRINT)->load_config(new_print_config);
    plat->on_config_change(new_print_config);
    plat->changed_objects(objs_idx);
    this->gui_app->get_tab(Preset::TYPE_PRINT)->update_dirty();
    plat->is_preview_shown();

    //update everything, easier to code.
    ObjectList* obj = this->gui_app->obj_list();
    obj->update_after_undo_redo();

    plat->reslice();
    plat->select_view_3D("Preview");
}
} // namespace GUI
} // namespace Slic3r
