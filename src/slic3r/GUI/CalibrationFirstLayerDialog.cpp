#include "CalibrationFirstLayerDialog.hpp"
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

void CalibrationFirstLayerDialog::create_buttons(wxStdDialogButtonSizer* buttons){
    Plater* plat = this->main_frame->plater();
    const BuildVolume::Type type = plat->build_volume().type();

    bool isRect = (type == BuildVolume::Type::Rectangle);
    bool isCirc = (type == BuildVolume::Type::Circle);

    if (isRect){
        wxString choices_quantity[] = {"5","9"};
        quantity = new wxComboBox(this, wxID_ANY, wxString{ "5" }, wxDefaultPosition, wxDefaultSize, 2, choices_quantity);
        quantity->SetToolTip(_L("You can choose the size of the cube."
            " It's a simple scale, you can modify it in the right panel yourself if you prefer. It's just quicker to select it here."));
        quantity->SetSelection(0);
        buttons->Add(new wxStaticText(this, wxID_ANY, _L("Quantity:")));
        buttons->AddSpacer(10);
        buttons->Add(quantity);
    } else if (isCirc){
        wxString choices_quantity[] = {"5"};
        quantity = new wxComboBox(this, wxID_ANY, wxString{ "5" }, wxDefaultPosition, wxDefaultSize, 1, choices_quantity);
        quantity->SetToolTip(_L("You can choose the size of the cube."
            " It's a simple scale, you can modify it in the right panel yourself if you prefer. It's just quicker to select it here."));
        quantity->SetSelection(0);
    } else return ;

    buttons->AddSpacer(40);
    wxButton* bt = new wxButton(this, wxID_FILE1, _(L("Generate")));
    bt->Bind(wxEVT_BUTTON, &CalibrationFirstLayerDialog::create_geometry_v, this);
    buttons->Add(bt);
}

void CalibrationFirstLayerDialog::create_geometry() {
    Plater* plat = this->main_frame->plater();
    Model& model = plat->model();
    plat->new_project();

    /// --- load central square ---
    std::vector<size_t> objs_idx = plat->load_files(std::vector<std::string>{
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string()
            }, true, false, false);

    int idx_quantity= quantity->GetSelection();

    assert(objs_idx.size() == 1);
    const DynamicPrintConfig* printConfig = this->gui_app->get_tab(Preset::TYPE_PRINT)->get_config();
    const DynamicPrintConfig* printerConfig = this->gui_app->get_tab(Preset::TYPE_PRINTER)->get_config();
    const ConfigOptionPoints* bed_shape = printerConfig->option<ConfigOptionPoints>("bed_shape");

    const BuildVolume::Type type = plat->build_volume().type();

    bool isCirc = (type == BuildVolume::Type::Circle);

    Vec2d bed_size = BoundingBoxf(bed_shape->values).size();
    Vec2d bed_min = BoundingBoxf(bed_shape->values).min;

    /// --- scale the squares according to bed quantities ---
    float bed_dim_x = bed_size.x() - bed_min.x();
    float bed_dim_y = bed_size.y() - bed_min.y();

    double radius = unscaled<double>(plat->build_volume().circle().radius);

    if (isCirc){
        bed_dim_x = 2.0 * radius * 0.90;
        bed_dim_y = bed_dim_x;
    }

    float xyScale = 1.0;
    float zScale = 1.0;
    float sqSize = 20.0;
    float xyOffset = 5;
    if (bed_dim_x < 100 || bed_dim_y < 100) {
        xyScale = 0.5;
    } else if (bed_dim_x > 150 && bed_dim_y > 150) {
        xyScale = 1.5;
    }
    sqSize = xyScale * sqSize;

    /// --- scale in z direction according to layer height and nozzle diameter ---
    const ConfigOptionFloats* nozzle_diameter_config = printerConfig->option<ConfigOptionFloats>("nozzle_diameter");
    assert(nozzle_diameter_config->values.size() > 0);
    float nozzle_diameter = nozzle_diameter_config->values[0];

    const ConfigOptionFloatOrPercent* first_layer_height_setting = printConfig->option<ConfigOptionFloatOrPercent>("first_layer_height");
    double first_layer_height = first_layer_height_setting->get_abs_value(nozzle_diameter);

    zScale = std::max(first_layer_height, nozzle_diameter / 2.0) / 0.2;

    model.objects[objs_idx[0]]->scale(xyScale, xyScale, zScale);

    float offset = xyOffset + sqSize / 2.0;
    float xShift = -bed_dim_x / 2.0 + offset;
    float yShift = 0.0;
    float zShift = 0.0;

    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
    xShift = +bed_dim_x / 2.0 - offset;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
    xShift = 0.0;
    yShift = -bed_dim_y / 2.0 + offset;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
    yShift = +bed_dim_y / 2.0 - offset;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});

    if (idx_quantity == 1){
        xShift = -bed_dim_x / 2.0 + offset;
        yShift = +bed_dim_y / 2.0 - offset;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
        yShift = -bed_dim_y / 2.0 + offset;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
        xShift = +bed_dim_x / 2.0 - offset;
        yShift = +bed_dim_y / 2.0 - offset;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
        yShift = -bed_dim_y / 2.0 + offset;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "square.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xyScale , xyScale, zScale});
    }

    /// --- scale the lines to desired length
    float lengthx = (bed_dim_x - 3.0 * sqSize - 2.0 * xyOffset) / 2.0;
    float xScale = lengthx / 100.0;
    float lengthy = (bed_dim_y - 3.0 * sqSize - 2.0 * xyOffset) / 2.0;
    float yScale = 2.0 * nozzle_diameter * 1.1; // we want to have 2 lines
    zScale = 1.0;
    xShift = -(sqSize + lengthx) / 2.0;
    yShift = 0.0;
    zShift = 0.0;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_x.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
    xShift = +(sqSize + lengthx) / 2.0;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_x.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});

    if (idx_quantity == 1){
        xShift = -(sqSize + lengthx) / 2.0;
        yShift = lengthy + sqSize;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_x.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
        yShift = -(lengthy + sqSize);
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_x.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
        xShift = +(sqSize + lengthx) / 2.0;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_x.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
        yShift = lengthy + sqSize;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_x.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
    }

    xScale = 2.0 * nozzle_diameter * 1.1 ; // we want to have 2 lines
    yScale = lengthy / 100.0;
    zScale = 1.0;
    xShift = 0.0;
    yShift = -(sqSize + lengthy) / 2.0;
    zShift = 0.0;

    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
    yShift = +(sqSize + lengthy) / 2.0;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});

    if (idx_quantity == 1){
        xShift = lengthx + sqSize;
        yShift = -(sqSize + lengthy) / 2.0;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
        yShift = +(sqSize + lengthy) / 2.0;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
        xShift = -(lengthx + sqSize);
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
        yShift = -(sqSize + lengthy) / 2.0;
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});
    }

    xScale = 1.0;
    yScale = 0.05;
    zScale = 1.0;
    xShift = - bed_dim_x / 2 + xyOffset +sqSize + 10;
    yShift = 2.5;
    zShift = 0.0;
    add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" /"first_layer"/ "line_y.3mf").string(), Vec3d{xShift, yShift, zShift}, Vec3d{ xScale , yScale, zScale});

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
