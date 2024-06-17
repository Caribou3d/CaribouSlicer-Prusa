#ifndef slic3r_GUI_CalibrationFirstLayerDialog_hpp_
#define slic3r_GUI_CalibrationFirstLayerDialog_hpp_

#include "CalibrationAbstractDialog.hpp"

namespace Slic3r {
namespace GUI {

class CalibrationFirstLayerDialog : public CalibrationAbstractDialog
{

public:
    CalibrationFirstLayerDialog(GUI_App* app, MainFrame* mainframe) : CalibrationAbstractDialog(app, mainframe, "Calibration walls") { create(boost::filesystem::path("calibration") / "first_layer", "first_layer.html"); }
    virtual ~CalibrationFirstLayerDialog(){ }

protected:
    void create_buttons(wxStdDialogButtonSizer* sizer) override;
    void create_geometry();
    void create_geometry_v(wxCommandEvent& event_args) { create_geometry(); }

    wxComboBox* quantity;
    wxComboBox* calibrate;

};

} // namespace GUI
} // namespace Slic3r

#endif
