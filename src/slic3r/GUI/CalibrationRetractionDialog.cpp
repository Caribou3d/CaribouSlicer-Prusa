#include "CalibrationRetractionDialog.hpp"
#include "I18N.hpp"
#include "libslic3r/Model.hpp"
#include "libslic3r/Utils.hpp"
#include "libslic3r/AppConfig.hpp"
#include "Jobs/ArrangeJob.hpp"
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

void CalibrationRetractionDialog::create_buttons(wxStdDialogButtonSizer* buttons){
    wxString choices_steps[] = { "0.1","0.2","0.5","1","2" };
    steps = new wxComboBox(this, wxID_ANY, wxString{ "0.2" }, wxDefaultPosition, wxDefaultSize, 5, choices_steps);
    steps->SetToolTip(_L("Each militer add this value to the retraction value."));
    steps->SetSelection(1);
    wxString choices_nb[] = { "2","4","6","8","10","15","20","25" };
    nb_steps = new wxComboBox(this, wxID_ANY, wxString{ "15" }, wxDefaultPosition, wxDefaultSize, 8, choices_nb);
    nb_steps->SetToolTip(_L("Select the number milimeters for the tower."));
    nb_steps->SetSelection(5);
    //wxString choices_start[] = { "current","260","250","240","230","220","210" };
    //start_step = new wxComboBox(this, wxID_ANY, wxString{ "current" }, wxDefaultPosition, wxDefaultSize, 7, choices_start);
    //start_step->SetToolTip(_(L("Select the highest temperature to test for.")));
    //start_step->SetSelection(0);
    const DynamicPrintConfig* filament_config = this->gui_app->get_tab(Preset::TYPE_FILAMENT)->get_config();
    int temp = int((2 + filament_config->option<ConfigOptionInts>("temperature")->get_at(0)) / 5) * 5;
    auto size = wxSize(4 * em_unit(), wxDefaultCoord);
    temp_start = new wxTextCtrl(this, wxID_ANY, std::to_string(temp), wxDefaultPosition, size);
    temp_start->SetToolTip(_L("Note that only Multiple of 5 can be engraved in the part"));
    wxString choices_decr[] = { _L("one test"),_L("2x 10°"),_L("3x 10°"), _L("4x 10°"), _L("3x 5°"), _L("5x 5°") };
    decr_temp = new wxComboBox(this, wxID_ANY, wxString{ "current" }, wxDefaultPosition, wxDefaultSize, 6, choices_decr);
    decr_temp->SetToolTip(_L("Select the number tower to print, and by how many degrees C to decrease each time."));
    decr_temp->SetSelection(0);
    decr_temp->SetEditable(false);

    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Step:")));
    buttons->Add(steps);
    buttons->AddSpacer(15);
    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Height:")));
    buttons->Add(nb_steps);
    buttons->AddSpacer(20);

    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Start temp:")));
    buttons->Add(temp_start);
    buttons->AddSpacer(15);
    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Temp decr:")));
    buttons->Add(decr_temp);
    buttons->AddSpacer(20);

    wxButton* bt = new wxButton(this, wxID_FILE1, _L("Remove fil. slowdown"));
    bt->Bind(wxEVT_BUTTON, &CalibrationRetractionDialog::remove_slowdown, this);
    buttons->Add(bt);

    buttons->AddSpacer(30);

    bt = new wxButton(this, wxID_FILE1, _L("Generate"));
    bt->Bind(wxEVT_BUTTON, &CalibrationRetractionDialog::create_geometry, this);
    buttons->Add(bt);
}

void CalibrationRetractionDialog::remove_slowdown(wxCommandEvent& event_args) {

    const DynamicPrintConfig* filament_config = this->gui_app->get_tab(Preset::TYPE_FILAMENT)->get_config();
    DynamicPrintConfig new_filament_config = *filament_config; //make a copy

    const ConfigOptionFloats *fil_conf = filament_config->option<ConfigOptionFloats>("slowdown_below_layer_time");
    ConfigOptionFloats *new_fil_conf = new ConfigOptionFloats();
//    new_fil_conf->default_value = 5;
//   new_fil_conf->values = fil_conf->values;
//    new_fil_conf->values[0] = 0;
//    new_filament_config.set_key_value("slowdown_below_layer_time", new_fil_conf);

//    fil_conf = filament_config->option<ConfigOptionFloats>("fan_below_layer_time"); new_fil_conf = new ConfigOptionFloats();
//    new_fil_conf->default_value = 60;
//    new_fil_conf->values = fil_conf->values;
//    new_fil_conf->values[0] = 0;
//    new_filament_config.set_key_value("fan_below_layer_time", new_fil_conf);

    this->gui_app->get_tab(Preset::TYPE_FILAMENT)->load_config(new_filament_config);
    this->main_frame->plater()->on_config_change(new_filament_config);
    this->gui_app->get_tab(Preset::TYPE_FILAMENT)->update_dirty();

}

void CalibrationRetractionDialog::create_geometry(wxCommandEvent& event_args) {
   Plater* plat = this->main_frame->plater();
    Model& model = plat->model();
    plat->new_project();

    //GLCanvas3D::set_warning_freeze(true);
    bool autocenter = gui_app->app_config->get("autocenter") == "1";
    if (autocenter) {
        //disable aut-ocenter for this calibration.
        gui_app->app_config->set("autocenter", "0");
    }

    long nb_retract = 1;
    if (!nb_steps->GetValue().ToLong(&nb_retract)) {
        nb_retract = 15;
    }
    size_t nb_items = 1;
    if (decr_temp->GetSelection() == 1) {
        nb_items = 2;
    } else if (decr_temp->GetSelection() == 2 || decr_temp->GetSelection() == 4) {
        nb_items = 3;
    } else if (decr_temp->GetSelection() == 3) {
        nb_items = 4;
    } else if (decr_temp->GetSelection() == 5) {
        nb_items = 5;
    }
    int temp_decr = (decr_temp->GetSelection() < 4) ? 10 : 5;


    std::vector<std::string> items;
    for (size_t i = 0; i < nb_items; i++)
        items.emplace_back((boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "retraction" / "retraction_base.3mf").string());
    std::vector<size_t> objs_idx = plat->load_files(items, true, false, false);


    assert(objs_idx.size() == nb_items);
    const DynamicPrintConfig* printConfig = this->gui_app->get_tab(Preset::TYPE_PRINT)->get_config();
    const DynamicPrintConfig* printerConfig = this->gui_app->get_tab(Preset::TYPE_PRINTER)->get_config();
    const DynamicPrintConfig* filamentConfig = this->gui_app->get_tab(Preset::TYPE_FILAMENT)->get_config();
    DynamicPrintConfig full_printConfig;
    full_printConfig.apply(*printConfig);
    full_printConfig.apply(*printerConfig);
    full_printConfig.apply(*filamentConfig);

    double retraction_start = 0;
    std::string str = temp_start->GetValue().ToStdString();
    int temp = int((2 + filamentConfig->option<ConfigOptionInts>("temperature")->get_at(0)) / 5) * 5;
    if (str.find_first_not_of("0123456789") == std::string::npos)
        temp = std::atoi(str.c_str());

    double retraction_steps = 0.01;
    if (!steps->GetValue().ToDouble(&retraction_steps)) {
        retraction_steps = 0.1;
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
    first_layer_height = nozzle_diameter / 2; //TODO remove and use the user's first_layer_height
    double layer_height = nozzle_diameter / 2.;
    first_layer_height = std::max(first_layer_height, nozzle_diameter / 2.);

    float scale = nozzle_diameter / 0.4;
    //do scaling

     if (scale < 0.9 || 1.2 < scale) {
        for (size_t i = 0; i < nb_items; i++)
            model.objects[objs_idx[i]]->scale(scale, scale, scale);
    }

    //add sub-part after scale
    float zscale_number = (first_layer_height + layer_height) / 0.4;
    std::vector < std::vector<ModelObject*>> part_tower;
    for (size_t id_item = 0; id_item < nb_items; id_item++) {
        part_tower.emplace_back();
        int mytemp = temp - temp_decr * id_item;
        if (mytemp <= 285 && mytemp >= 180 && mytemp % 5 == 0) {
            add_part(model.objects[objs_idx[id_item]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "retraction" / ("t" + std::to_string(mytemp) + ".3mf")).string(),
                Vec3d{ 0,0, 0 }, Vec3d{ scale,scale,scale });
//            model.objects[objs_idx[id_item]]->volumes[1]->rotate(PI / 2, Vec3d(0, 0, 1));
//            model.objects[objs_idx[id_item]]->volumes[1]->rotate(-PI / 2, Vec3d(1, 0, 0));
        }
        for (int num_retract = 0; num_retract < nb_retract; num_retract++) {
            part_tower.back().push_back(add_part(model.objects[objs_idx[id_item]],
                (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "retraction" / "retraction_pillar.3mf").string(),
                Vec3d{ 0,0,scale * 0.7 - 0.3 + scale * num_retract }, Vec3d{ scale,scale,scale }));
        }
    }

    /// --- translate ---;
    bool has_to_arrange = false;
    const ConfigOptionFloat* extruder_clearance_radius = printConfig->option<ConfigOptionFloat>("extruder_clearance_radius");
    const ConfigOptionPoints* bed_shape = printerConfig->option<ConfigOptionPoints>("bed_shape");
    const float brim_width = std::max(printConfig->option<ConfigOptionFloat>("brim_width")->value, nozzle_diameter * 5.);
    Vec2d bed_size = BoundingBoxf(bed_shape->values).size();
    Vec2d bed_min = BoundingBoxf(bed_shape->values).min;
    float offset = 4 + 26 * scale * 1 + extruder_clearance_radius->value + brim_width + (brim_width > extruder_clearance_radius->value ? brim_width - extruder_clearance_radius->value : 0);

     model.objects[objs_idx[0]]->translate({ -40, 0, 0 });
     model.objects[objs_idx[1]]->translate({ +40, 0, 0 });

/*
    /// --- custom config ---
    for (size_t i = 0; i < nb_items; i++) {
        //speed
        double perimeter_speed = full_print_config.get_computed_value("perimeter_speed");
        double external_perimeter_speed = full_print_config.get_computed_value("external_perimeter_speed");
        //brim to have some time to build up pressure in the nozzle
        model.objects[objs_idx[i]]->config.set_key_value("brim_width", new ConfigOptionFloat(0));
        model.objects[objs_idx[i]]->config.set_key_value("perimeters", new ConfigOptionInt(2));
        model.objects[objs_idx[i]]->config.set_key_value("external_perimeters_first", new ConfigOptionBool(false));
        model.objects[objs_idx[i]]->config.set_key_value("bottom_solid_layers", new ConfigOptionInt(0));
        model.objects[objs_idx[i]]->volumes[0]->config.set_key_value("bottom_solid_layers", new ConfigOptionInt(2));
        //model.objects[objs_idx[i]]->volumes[1]->config.set_key_value("bottom_solid_layers", new ConfigOptionInt(2));
        model.objects[objs_idx[i]]->config.set_key_value("top_solid_layers", new ConfigOptionInt(0));
        model.objects[objs_idx[i]]->config.set_key_value("fill_density", new ConfigOptionPercent(0));
        //model.objects[objs_idx[i]]->config.set_key_value("fill_pattern", new ConfigOptionEnum<InfillPattern>(ipRectilinear));
        model.objects[objs_idx[i]]->config.set_key_value("only_one_perimeter_top", new ConfigOptionBool(false));
        model.objects[objs_idx[i]]->config.set_key_value("overhangs_width_speed", new ConfigOptionFloatOrPercent(0,false));
        model.objects[objs_idx[i]]->config.set_key_value("thin_walls", new ConfigOptionBool(true));
        model.objects[objs_idx[i]]->config.set_key_value("thin_walls_min_width", new ConfigOptionFloatOrPercent(2,true));
        model.objects[objs_idx[i]]->config.set_key_value("gap_fill_enabled", new ConfigOptionBool(false));
        model.objects[objs_idx[i]]->config.set_key_value("first_layer_height", new ConfigOptionFloatOrPercent(nozzle_diameter / 2., false));
        model.objects[objs_idx[i]]->config.set_key_value("layer_height", new ConfigOptionFloat(nozzle_diameter / 2.));
        //temp
        model.objects[objs_idx[i]]->config.set_key_value("print_temperature", new ConfigOptionInt(int(temp - temp_decr * i)));
        //set retraction override
        size_t num_part = 0;
        const int mytemp = temp - temp_decr * i;
        const int extra_vol = (mytemp <= 285 && mytemp >= 180 && mytemp % 5 == 0) ? 2 : 1;
        for (ModelObject* part : part_tower[i]) {
            model.objects[objs_idx[i]]->volumes[num_part + extra_vol]->config.set_key_value("print_retract_length", new ConfigOptionFloat(retraction_start + num_part * retraction_steps));
            model.objects[objs_idx[i]]->volumes[num_part + extra_vol]->config.set_key_value("small_perimeter_speed", new ConfigOptionFloatOrPercent(external_perimeter_speed, false));
            model.objects[objs_idx[i]]->volumes[num_part + extra_vol]->config.set_key_value("perimeter_speed", new ConfigOptionFloatOrPercent(std::min(external_perimeter_speed, perimeter_speed), false));
            model.objects[objs_idx[i]]->volumes[num_part + extra_vol]->config.set_key_value("external_perimeter_speed", new ConfigOptionFloatOrPercent(external_perimeter_speed, false));
            //model.objects[objs_idx[i]]->volumes[num_part + extra_vol]->config.set_key_value("small_perimeter_speed", new ConfigOptionFloatOrPercent(external_perimeter_speed, false));
            //model.objects[objs_idx[i]]->volumes[num_part + extra_vol]->config.set_key_value("infill_speed", new ConfigOptionFloatOrPercent(std::min(print_config->option<ConfigOptionFloatOrPercent>("infill_speed")->value, 10.*scale)), false);
            num_part++;
        }
    }

    if (nb_items > 1) {
        DynamicPrintConfig new_print_config = *print_config; //make a copy
        new_print_config.set_key_value("complete_objects", new ConfigOptionBool(true));
        //if skirt, use only one
        if (print_config->option<ConfigOptionInt>("skirts")->getInt() > 0 && print_config->option<ConfigOptionInt>("skirt_height")->getInt() > 0) {
            new_print_config.set_key_value("complete_objects_one_skirt", new ConfigOptionBool(true));
        }
        this->gui_app->get_tab(Preset::TYPE_FFF_PRINT)->load_config(new_print_config);
        this->gui_app->get_tab(Preset::TYPE_FFF_PRINT)->update_dirty();
        plat->on_config_change(new_print_config);
    }
*/
    /// --- main config, please modify object config when possible ---
    DynamicPrintConfig new_printConfig = *printConfig; //make a copy

    new_printConfig.set_key_value("complete_objects", new ConfigOptionBool(true));

    new_printConfig.set_key_value("skirts", new ConfigOptionInt(2));
    new_printConfig.set_key_value("skirt_distance", new ConfigOptionFloat(1.0));

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

    }

} // namespace GUI
} // namespace Slic3r
