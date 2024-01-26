#ifndef slic3r_GUI_CalibrationFirstLayerPatchDialog_hpp_
#define slic3r_GUI_CalibrationFirstLayerPatchDialog_hpp_

#include "CalibrationAbstractDialog.hpp"

namespace Slic3r {
namespace GUI {

class CalibrationFirstLayerPatchDialog : public CalibrationAbstractDialog
{

public:
    CalibrationFirstLayerPatchDialog(GUI_App* app, MainFrame* mainframe) : CalibrationAbstractDialog(app, mainframe, "Calibration walls") { create(boost::filesystem::path("calibration") / "walls", "walls.html"); }
    virtual ~CalibrationFirstLayerPatchDialog(){ }

protected:
    void create_buttons(wxStdDialogButtonSizer* sizer) override;
    void create_geometry();
    void create_geometry_v(wxCommandEvent& event_args) { create_geometry(); }

    wxComboBox* dimension;
    wxComboBox* calibrate;

};

} // namespace GUI
} // namespace Slic3r

#endif
