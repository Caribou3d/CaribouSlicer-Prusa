#include "CalibrationFlowDialog.hpp"
#include "I18N.hpp"
#include "libslic3r/Model.hpp"
#include "libslic3r/Utils.hpp"
#include "libslic3r/AppConfig.hpp"
//#include "Jobs/ArrangeJob.hpp"
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

void CalibrationFlowDialog::create_buttons(wxStdDialogButtonSizer* buttons){
    wxButton* bt = new wxButton(this, wxID_FILE1, _L("Generate 10% intervals around current value"));
    bt->Bind(wxEVT_BUTTON, &CalibrationFlowDialog::create_geometry_10, this);
    buttons->Add(bt);
    bt = new wxButton(this, wxID_FILE2, _L("Generate 2% intervals below current value"));
    bt->Bind(wxEVT_BUTTON, &CalibrationFlowDialog::create_geometry_2_5, this);
    buttons->Add(bt);
}


void CalibrationFlowDialog::create_geometry(float start, float delta) {
    Plater* plat = this->main_frame->plater();
    Model& model = plat->model();

    plat->new_project();

    const DynamicPrintConfig* printConfig = this->gui_app->get_tab(Preset::TYPE_PRINT)->get_config();
    const DynamicPrintConfig* printerConfig = this->gui_app->get_tab(Preset::TYPE_PRINTER)->get_config();

    //GLCanvas3D::set_warning_freeze(true);
    bool autocenter = gui_app->app_config->get("autocenter") == "1";
    if (autocenter) {
        //disable auto-center for this calibration.
        gui_app->app_config->set("autocenter", "0");
    }

    std::vector<size_t> objs_idx;

    if (delta == 10.f && start == 80.f) {
        objs_idx = plat->load_files(std::vector<std::string>{
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "m20.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "m10.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "0.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "p10.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "p20.3mf").string()}, true, false, false);
    } else if (delta == 2.f && start == 92.f) {
        objs_idx = plat->load_files(std::vector<std::string>{
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "m8.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "m6.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "m4.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "m2.3mf").string(),
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "0.3mf").string()}, true, false, false);
    }

    assert(objs_idx.size() == 5);

    for (size_t i = 0; i < 5; i++) {
        add_part(model.objects[objs_idx[i]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_flow" / "O.3mf").string(), Vec3d{ 0.0,-5,0.5 + 0.1}, Vec3d{1.0, 1.0, 1.0}); // base: 0.2mm height
    }

    for (size_t i = 0; i < 5; i++) {
        model.objects[objs_idx[i]]->config.set_key_value("print_extrusion_multiplier", new ConfigOptionPercent(start + (float)i * delta));
    }

    const ConfigOptionFloat* extruder_clearance_radius = printConfig->option<ConfigOptionFloat>("extruder_clearance_radius");
    double xyshift = 1.2 * extruder_clearance_radius->value;

    if (delta == 10.f && start == 80.f) {
        model.objects[objs_idx[0]]->translate({ -xyshift, xyshift, 0 });
        model.objects[objs_idx[1]]->translate({ xyshift, xyshift, 0 });
        model.objects[objs_idx[3]]->translate({ -xyshift, -xyshift, 0 });
        model.objects[objs_idx[4]]->translate({ xyshift, -xyshift, 0 });
    } else if (delta == 2.f && start == 92.f) {
        model.objects[objs_idx[0]]->translate({ -xyshift, xyshift, 0 });
        model.objects[objs_idx[1]]->translate({ xyshift, xyshift, 0 });
        model.objects[objs_idx[2]]->translate({ xyshift, -xyshift, 0 });
        model.objects[objs_idx[3]]->translate({ -xyshift, -xyshift, 0 });
    }

   /// --- scale ---
    // model is created for a 0.4 nozzle, scale xy with nozzle size.
    const ConfigOptionFloats* nozzle_diameter_config = printerConfig->option<ConfigOptionFloats>("nozzle_diameter");
    assert(nozzle_diameter_config->values.size() > 0);
    float nozzle_diameter = nozzle_diameter_config->values[0];
    float xyScale = nozzle_diameter / 0.4;
    //scale z to have 6 layers
    const ConfigOptionFloatOrPercent* first_layer_height_setting = printConfig->option<ConfigOptionFloatOrPercent>("first_layer_height");
    double first_layer_height = first_layer_height_setting->get_abs_value(nozzle_diameter);
    double layer_height = nozzle_diameter / 2.;
//    layer_height = check_z_step(layer_height, printerConfig->option<ConfigOptionFloat>("z_step")->value); // If z_step is not 0 the slicer will scale to the nearest multiple of z_step so account for that here

    std::cout << layer_height << "  " << first_layer_height << std::endl;

    first_layer_height = std::max(first_layer_height, nozzle_diameter / 2.);

    float zscale = first_layer_height + 5 * layer_height;

    std::cout << "scale: " << xyScale << "  " << zscale << std::endl;

    /// --- custom config ---
    for (size_t i = 0; i < 5; i++) {
        //brim to have some time to build up pressure in the nozzle
//        model.objects[objs_idx[i]]->config.set_key_value("brim_width", new ConfigOptionFloat(brim_width));
    model.objects[objs_idx[i]]->config.set_key_value("perimeters", new ConfigOptionInt(1));
    model.objects[objs_idx[i]]->config.set_key_value("fill_density", new ConfigOptionPercent(10));
    model.objects[objs_idx[i]]->config.set_key_value("top_solid_layers", new ConfigOptionInt(100));
    model.objects[objs_idx[i]]->config.set_key_value("bottom_solid_layers", new ConfigOptionInt(5));
    model.objects[objs_idx[i]]->config.set_key_value("brim_width", new ConfigOptionFloat(1.6));
    model.objects[objs_idx[i]]->config.set_key_value("layer_height", new ConfigOptionFloat(layer_height));
    model.objects[objs_idx[i]]->config.set_key_value("first_layer_height", new ConfigOptionFloatOrPercent(first_layer_height, false));

       //disable ironing post-process
        model.objects[objs_idx[i]]->config.set_key_value("ironing", new ConfigOptionBool(false));
    }

    //update plater
    //GLCanvas3D::set_warning_freeze(false);

    DynamicPrintConfig new_printConfig = *printConfig; //make a copy

    new_printConfig.set_key_value("complete_objects", new ConfigOptionBool(true));


    /// --- main config, please modify object config when possible ---
    new_printConfig.set_key_value("skirts", new ConfigOptionInt(2));
    new_printConfig.set_key_value("skirt_distance", new ConfigOptionFloat(1.0));
    new_printConfig.set_key_value("skirt_height", new ConfigOptionInt(1));

    //update plater
    this->gui_app->get_tab(Preset::TYPE_PRINT)->load_config(new_printConfig);
    plat->on_config_change(new_printConfig);
    plat->changed_objects(objs_idx);
    this->gui_app->get_tab(Preset::TYPE_PRINT)->update_dirty();
    plat->is_preview_shown();
    ObjectList* obj = this->gui_app->obj_list();
    obj->update_after_undo_redo();

    plat->reslice();
    plat->select_view_3D("Preview");


//    if (autocenter) {
//        //re-enable auto-center after this calibration.
//        gui_app->app_config->set("autocenter", "1");
//    }
}

} // namespace GUI
} // namespace Slic3r
