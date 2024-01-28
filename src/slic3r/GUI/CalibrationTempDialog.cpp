#include "CalibrationTempDialog.hpp"
#include "I18N.hpp"
#include "libslic3r/Utils.hpp"
#include "libslic3r/CustomGCode.hpp"
#include "libslic3r/Model.hpp"
#include "libslic3r/AppConfig.hpp"
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

void CalibrationTempDialog::create_buttons(wxStdDialogButtonSizer* buttons){
    wxString choices_steps[] = { "5","10" };
    steps = new wxComboBox(this, wxID_ANY, wxString{ "10" }, wxDefaultPosition, wxDefaultSize, 2, choices_steps);
    steps->SetToolTip(_L("Select the step in celcius between two tests."));
    steps->SetSelection(1);

    wxString choices_temp[] = { "170","180","190","200","210","220","230","240","250","260","270","280","290" };
    temp_low = new wxComboBox(this, wxID_ANY, wxString{ "200" }, wxDefaultPosition, wxDefaultSize, 13, choices_temp);
    temp_low->SetToolTip(_L("Select the lower temperature."));
    temp_low->SetSelection(4);

    temp_high = new wxComboBox(this, wxID_ANY, wxString{ "200" }, wxDefaultPosition, wxDefaultSize, 13, choices_temp);
    temp_high->SetToolTip(_L("Select the higher temperature."));
    temp_high->SetSelection(8);

    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Lower temp:")));
    buttons->AddSpacer(15);
    buttons->Add(temp_low);
    buttons->AddSpacer(15);
    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Upper temp:")));
    buttons->AddSpacer(15);
    buttons->Add(temp_high);
    buttons->AddSpacer(40);
    buttons->Add(new wxStaticText(this, wxID_ANY, _L("Steps:")));
    buttons->AddSpacer(15);
    buttons->Add(steps);
    buttons->AddSpacer(40);

    wxButton* bt = new wxButton(this, wxID_FILE1, _L("Generate"));
    bt->Bind(wxEVT_BUTTON, &CalibrationTempDialog::create_geometry, this);
    buttons->Add(bt);
}

void CalibrationTempDialog::create_geometry(wxCommandEvent& event_args) {
    Plater* plat = this->main_frame->plater();
    Model& model = plat->model();
    plat->new_project();

    //GLCanvas3D::set_warning_freeze(true);
    std::vector<size_t> objs_idx = plat->load_files(std::vector<std::string>{
            (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_temp" / "TempTowerBase.3mf").string()}, true, false, false);

    assert(objs_idx.size() == 1);
    const DynamicPrintConfig* printConfig = this->gui_app->get_tab(Preset::TYPE_PRINT)->get_config();
    const DynamicPrintConfig* filamentConfig = this->gui_app->get_tab(Preset::TYPE_FILAMENT)->get_config();
    const DynamicPrintConfig* printerConfig = this->gui_app->get_tab(Preset::TYPE_PRINTER)->get_config();

    // -- get temps
    const ConfigOptionInts* temperature_config = filamentConfig->option<ConfigOptionInts>("temperature");
    assert(temperature_config->values.size() >= 1);

    long temp_items_val1 = 1;
    temp_low->GetValue().ToLong(&temp_items_val1);

    long temp_items_val2 = 1;
    temp_high->GetValue().ToLong(&temp_items_val2);

    long temp_low  = std::min(temp_items_val1,temp_items_val2);
    long temp_high = std::max(temp_items_val1,temp_items_val2);

    //int16_t temperature = 5 * (temperature_config->values[0] / 5);
    long step_temp = 5;
    steps->GetValue().ToLong(&step_temp);

    std::cout << "temps: " << temp_low << "  " << temp_high << "  " << step_temp << std::endl;

    //size_t nb_items = 1 + nb_items_up + nb_items_down;
    //start at the highest temp
    //temperature = temperature + step_temp * nb_items_up;

    /// --- scale ---
    //model is created for a 0.4 nozzle, scale xy with nozzle size.
    const ConfigOptionFloats* nozzle_diameter_config = printerConfig->option<ConfigOptionFloats>("nozzle_diameter");
    assert(nozzle_diameter_config->values.size() > 0);
    float nozzle_diameter = nozzle_diameter_config->values[0];
    float xyzScale = nozzle_diameter / 0.4;
    //do scaling
    if (xyzScale < 0.9 || 1.1 < xyzScale) {
        model.objects[objs_idx[0]]->scale(1.0, 1.0, xyzScale);
    } else {
        xyzScale = 1;
        model.objects[objs_idx[0]]->scale(1.0, 1.0, xyzScale);
    }

    int16_t temp1 = temp_low;
    int16_t temp2 = temp_high;


    // add other objects

    float zshift = 1.4 * xyzScale + 4.3;
    std::cout << "zshift " << zshift << std::endl;

    while (temp2 >= temp1){
        add_part(model.objects[objs_idx[0]], (boost::filesystem::path(Slic3r::resources_dir()) / "calibration" / "filament_temp" / (std::to_string(temp2) + ".3mf")).string(),
                Vec3d{ 0.0,0.0,zshift }, Vec3d{ 1.0,1.0,1.0 });
        temp2 = temp2 - step_temp;
        zshift = zshift + 10;
    }



    /// --- main config, please modify object config when possible ---
    DynamicPrintConfig new_printConfig = *printConfig; //make a copy
    new_printConfig.set_key_value("complete_objects", new ConfigOptionBool(false));

/*
    /// -- generate the heat change gcode
    //std::string str_layer_gcode = "{if layer_num > 0 and layer_z  <= " + std::to_string(2 * xyzScale) + "}\nM104 S" + std::to_string(temperature - (int8_t)nb_delta * 5);
    //    double print_z, std::string gcode,int extruder, std::string color
    double firstChangeHeight = printConfig->get_abs_value("first_layer_height", nozzle_diameter);
    //model.custom_gcode_per_print_z.gcodes.emplace_back(CustomGCode::Item{ firstChangeHeight + nozzle_diameter/2, CustomGCode::Type::Custom, -1, "", "M104 S" + std::to_string(temperature) + " ; ground floor temp tower set" });
    model.objects[objs_idx[0]]->config.set_key_value("print_temperature", new ConfigOptionInt(temperature));
    for (int16_t i = 1; i < nb_items; i++) {
        model.custom_gcode_per_print_z.gcodes.emplace_back(CustomGCode::Item{ (i * 10 * xyzScale), CustomGCode::Type::Custom , -1, "", "M104 S" + std::to_string(temperature - i * step_temp) + " ; floor " + std::to_string(i) + " of the temp tower set" });
        //str_layer_gcode += "\n{ elsif layer_z >= " + std::to_string(i * 10 * xyzScale) + " and layer_z <= " + std::to_string((1 + i * 10) * xyzScale) + " }\nM104 S" + std::to_string(temperature - (int8_t)nb_delta * 5 + i * 5);
    }
    //str_layer_gcode += "\n{endif}\n";
    //DynamicPrintConfig new_printer_config = *printerConfig; //make a copy
    //new_printer_config.set_key_value("layer_gcode", new ConfigOptionString(str_layer_gcode));
*/
    /// --- custom config ---
 /*   float brim_width = printConfig->option<ConfigOptionFloat>("brim_width")->value;
    if (brim_width < nozzle_diameter * 8) {
        model.objects[objs_idx[0]]->config.set_key_value("brim_width", new ConfigOptionFloat(nozzle_diameter * 8));
    }
    model.objects[objs_idx[0]]->config.set_key_value("brim_ears", new ConfigOptionBool(false));
    model.objects[objs_idx[0]]->config.set_key_value("perimeters", new ConfigOptionInt(1));
    model.objects[objs_idx[0]]->config.set_key_value("extra_perimeters_overhangs", new ConfigOptionBool(true));
    model.objects[objs_idx[0]]->config.set_key_value("bottom_solid_layers", new ConfigOptionInt(2));
    model.objects[objs_idx[0]]->config.set_key_value("top_solid_layers", new ConfigOptionInt(3));
    model.objects[objs_idx[0]]->config.set_key_value("gap_fill_enabled", new ConfigOptionBool(false));
    model.objects[objs_idx[0]]->config.set_key_value("thin_perimeters", new ConfigOptionPercent(100));
    model.objects[objs_idx[0]]->config.set_key_value("layer_height", new ConfigOptionFloat(nozzle_diameter / 2));
    model.objects[objs_idx[0]]->config.set_key_value("fill_density", new ConfigOptionPercent(7));
//    model.objects[objs_idx[0]]->config.set_key_value("solid_fill_pattern", new ConfigOptionEnum<InfillPattern>(ipRectilinearWGapFill));
//    model.objects[objs_idx[0]]->config.set_key_value("top_fill_pattern", new ConfigOptionEnum<InfillPattern>(ipRectilinearWGapFill));
    //disable ironing post-process, it only slow down things
    model.objects[objs_idx[0]]->config.set_key_value("ironing", new ConfigOptionBool(false));

*/
    //update plater
    //GLCanvas3D::set_warning_freeze(false);
    this->gui_app->get_tab(Preset::TYPE_PRINT)->load_config(new_printConfig);
    plat->on_config_change(new_printConfig);
    //this->gui_app->get_tab(Preset::TYPE_PRINTER)->load_config(new_printer_config);
    //plat->on_config_change(new_printer_config);
    plat->changed_objects(objs_idx);
    this->gui_app->get_tab(Preset::TYPE_PRINT)->update_dirty();
    //this->gui_app->get_tab(Preset::TYPE_PRINTER)->update_dirty();
    plat->is_preview_shown();
    //update everything, easier to code.
    ObjectList* obj = this->gui_app->obj_list();
    obj->update_after_undo_redo();

    plat->reslice();
    plat->select_view_3D("Preview");
}

} // namespace GUI
} // namespace Slic3r
