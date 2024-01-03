#!/bin/bash

if [ -z "$SCRIPT_PATH" ]; then
	SCRIPT_PATH="$( cd "$(dirname "$0")" ; pwd -P )"
fi

#=====================================================================================
#
# modify profiles

sed -i 's|config_update_url = https://caribou3d.com/CaribouSlicer/repository/vendors/Caribou|config_update_url =|' $SCRIPT_PATH/resources/profiles/Caribou.ini

find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed -i 's/caribou3d.com\/CaribouSlicer\/repository\/vendors/files.prusa3d.com\/wp-content\/uploads\/repository\/PrusaSlicer-settings-master\/live/g' {} +
find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed -i 's/ CaribouSlicer/ PrusaSlicer/g' {} +

sed -i -e 's|"http://caribou3d.com/CaribouSlicer/repository/"|"http://files.prusa3d.com/wp-content/uploads/repository/"|g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp
sed -i -e 's|"https://caribou3d.com/CaribouSlicer/repository/"|"https://files.prusa3d.com/wp-content/uploads/repository/"|g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp

#sed -i 's|# changelog_url =|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles\&lng=%1%|g' $SCRIPT_PATH/resources/profiles/*.ini
find $SCRIPT_PATH/resources/profiles -type f ! -name "Trimaker.ini" -exec sed -i 's|# changelog_url =|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles\&lng=%1%|g' {} +

#=====================================================================================
#
# set version

sed -i '/set(SLIC3R_BUILD_NR "23279")/d' version.inc

#=====================================================================================
#
# modify localization files

rm -f $SCRIPT_PATH/resources/localization/CaribouSlicer.pot
find $SCRIPT_PATH/resources/localization -type f -name 'Caribou*' -delete

sed -i 's/CaribouSlicer.pot/PrusaSlicer.pot/g' $SCRIPT_PATH/CMakeLists.txt

sed -i 's/caribouslicer_copy/prusaslicer_copy/g' $SCRIPT_PATH/CMakeLists.txt
sed -i 's/CaribouSlicer_app/PrusaSlicer_app/g' $SCRIPT_PATH/CMakeLists.txt

sed -i 's/CaribouSlicer_\$/PrusaSlicer_\$/g' $SCRIPT_PATH/CMakeLists.txt
sed -i 's/CaribouSlicer\*.po/PrusaSlicer*.po/g' $SCRIPT_PATH/CMakeLists.txt
sed -i 's/CaribouSlicer_./PrusaSlicer_./g' $SCRIPT_PATH/CMakeLists.txt
sed -i 's/CaribouSlicer.mo/PrusaSlicer.mo/g' $SCRIPT_PATH/CMakeLists.txt

#exit
#=====================================================================================
#
# modify icons

sed -i -e "s/107C18/bf6637/g" $SCRIPT_PATH/resources/icons/timer_dot.svg

find $SCRIPT_PATH/resources/icons \( -name 'notification_clippy.svg' -o -name 'CaribouGcodeviewer.svg' -o -name 'PrusaSlicer.svg' -o -name 'error_tick_f.svg' -o -name 'error_tick.svg' -o -name 'exclamation.svg' -o -name 'notification_error.svg' -o -name 'notification_warning.svg' \) -prune -o -type f -name '*.svg' -exec sed -i 's/107C18/ED6B21/g' {} +
find $SCRIPT_PATH/resources/icons \( -name 'notification_clippy.svg' -o -name 'CaribouGcodeviewer.svg' -o -name 'PrusaSlicer.svg' -o -name 'error_tick_f.svg' -o -name 'error_tick.svg' -o -name 'exclamation.svg' -o -name 'notification_error.svg' -o -name 'notification_warning.svg' \) -prune -o -type f -name '*.svg' -exec sed -i 's/107c18/ed6b21/g' {} +

sed -i 's/ED0000/ED6B21/g' $SCRIPT_PATH/resources/icons/error_tick_f.svg  $SCRIPT_PATH/resources/icons/error_tick.svg $SCRIPT_PATH/resources/icons/exclamation.svg
sed -i 's/ed0000/ed6b21/g' $SCRIPT_PATH/resources/icons/notification_error.svg $SCRIPT_PATH/resources/icons/notification_warning.svg

sed -i -e 's/CaribouSlicer.png/PrusaSlicer.png/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed -i -e 's/CaribouSlicer.png/PrusaSlicer.png/g' $SCRIPT_PATH/CMakeLists.txt
sed -i -e 's/CaribouGcodeviewer.png/PrusaSlicer-gcodeviewer.png/g' $SCRIPT_PATH/CMakeLists.txt

sed -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp
sed -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/MsgDialog.cpp
sed -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/SendSystemInfoDialog.cpp

#exit
#=====================================================================================
#
# mainframe

sed -i 's/CaribouSlicer")/PrusaSlicer")/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i "s/CaribouSlicer_128/PrusaSlicer_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i "s/CaribouSlicer-mac_128/PrusaSlicer-mac_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i "s/CaribouGcodeviewer-mac_128/PrusaSlicer-gcodeviewer-mac_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i "s/CaribouGcodeviewer_/PrusaSlicer-gcodeviewer_/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i 's/github.com\/Caribou3d\/CaribouSlicer\/issues\/new\/choose/github.com\/prusa3d\/slic3r\/issues\/new/' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed -i 's/CaribouSlicer"/PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
#sed -i '/title += wxString(build_id);/a\    if (wxGetApp().is_editor())\n        title += (" " + _L("based on Slic3r"));\n' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp

#sed -i '/wxMenu\* helpMenu = new wxMenu();/a\
#\
#    append_menu_item(helpMenu, wxID_ANY, wxString::Format(_L("%s &Website"), SLIC3R_APP_NAME),\
#        wxString::Format(_L("Open the %s website in your browser"), SLIC3R_APP_NAME),\
#        [](wxCommandEvent&) { wxGetApp().open_web_page_localized("https://www.prusa3d.com/slicerweb"); });\
#    // TRN Item from "Help" menu\
#    append_menu_item(helpMenu, wxID_ANY, wxString::Format(_L("&Quick Start"), SLIC3R_APP_NAME),\
#        wxString::Format(_L("Open the %s website in your browser"), SLIC3R_APP_NAME),\
#        [](wxCommandEvent&) { wxGetApp().open_browser_with_warning_dialog("https://help.prusa3d.com/article/first-print-with-prusaslicer_1753", nullptr, false); });\
#    // TRN Item from "Help" menu\
#    append_menu_item(helpMenu, wxID_ANY, wxString::Format(_L("Sample &G-codes and Models"), SLIC3R_APP_NAME),\
#        wxString::Format(_L("Open the %s website in your browser"), SLIC3R_APP_NAME),\
#        [](wxCommandEvent&) { wxGetApp().open_browser_with_warning_dialog("https://help.prusa3d.com/article/sample-g-codes_529630", nullptr, false); });\
#    helpMenu->AppendSeparator();\
#    append_menu_item(helpMenu, wxID_ANY, _L("Prusa 3D &Drivers"), _L("Open the Prusa3D drivers download page in your browser"),\
#        [](wxCommandEvent&) { wxGetApp().open_web_page_localized("https://www.prusa3d.com/downloads"); });' src/slic3r/GUI/MainFrame.cpp

#exit
#=====================================================================================
#
# colors

sed -i -e "s/DARK_GREEN_PEN   = wxPen(wxColour(16, 124, 24));/DARK_ORANGE_PEN   = wxPen(wxColour(237, 107, 33));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed -i -e "s/GREEN_PEN        = wxPen(wxColour(24, 180, 36));/ORANGE_PEN        = wxPen(wxColour(253, 126, 66));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed -i -e "s/LIGHT_GREEN_PEN   = wxPen(wxColour(32, 216, 40));/LIGHT_ORANGE_PEN  = wxPen(wxColour(254, 177, 139));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed -i -e "s/GREEN_PEN/ORANGE_PEN/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed -i -e "s/GREEN_PEN/ORANGE_PEN/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.hpp

sed -i -e "s/ImVec4 orange_color			= ImVec4(0.063f, 0.353f, 0.094f, 1.0f);/ImVec4 orange_color			= ImVec4(.99f, .313f, .0f, 1.0f);/g" $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.cpp
#sed -i -e "s/ImVec4 orange_color = ImVec4(0.063f, 0.353f, 0.094f, 1.0f);/ImVec4 orange_color = ImVec4(.99f, .313f, .0f, 1.0f);g" $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.cpp

sed -i -e "s/return dark_mode() ? wxColour(24, 180, 36) : wxColour(16, 124, 24);/return dark_mode() ? wxColour(253, 111, 40) : wxColour(252, 77, 1);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed -i -e "s/wxColour(16, 124, 24)/wxColour(237, 107, 33)/g"  $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
 
sed -i -e "s/107C18/ED6B21/g" $SCRIPT_PATH/src/slic3r/GUI/BitmapCache.cpp
sed -i -e "s/107c18/ed6b21/g" $SCRIPT_PATH/src/slic3r/GUI/UnsavedChangesDialog.cpp

sed -i 's|static const ColorRGB GREENC()      { return { 0.06f, 0.49f, 0.09f }; }|static const ColorRGB ORANGE()      { return { 0.92f, 0.50f, 0.26f }; }|' $SCRIPT_PATH/src/libslic3r/Color.hpp
sed -i 's|static const ColorRGBA GREENC()      { return { 0.063f, 0.486f, 0.094f, 1.0f }; }|static const ColorRGBA ORANGE()      { return { 0.923f, 0.504f, 0.264f, 1.0f }; }|' $SCRIPT_PATH/src/libslic3r/Color.hpp

sed -i 's|const ImVec4 ImGuiWrapper::COL_GREENC_DARK.*|const ImVec4 ImGuiWrapper::COL_ORANGE_DARK       = { 0.67f, 0.36f, 0.19f, 1.0f };|; s|const ImVec4 ImGuiWrapper::COL_GREENC_LIGHT.*|const ImVec4 ImGuiWrapper::COL_ORANGE_LIGHT      = to_ImVec4(ColorRGBA::ORANGE());|' $SCRIPT_PATH/src/slic3r/GUI/ImGuiWrapper.cpp

sed -i -e 's/GREENC()/ORANGE()/g' $SCRIPT_PATH/src/slic3r/GUI/Gizmos/GLGizmoBase.hpp
sed -i -e 's/GREENC()/ORANGE()/g' $SCRIPT_PATH/src/slic3r/GUI/Selection.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed -i 's|COL_GREENC_|COL_ORANGE_|g' {} +

#sed -i '/find package\/resources\/localization -name "P\*.mo" -o -name "\*.txt" -o -name "P\*.pot" -type f -delete/d' src/platform/unix/BuildLinuxImage.sh.in

#exit
#=====================================================================================

sed -i 's/CaribouSlicer GU/PrusaSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed -i 's/CaribouSlicer GU/PrusaSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed -i 's/CaribouSlicer GU/PrusaSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/Preferences.cpp
sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h

sed -i '/set(SLIC3R_VIEWER "caribou-gcodeviewer")/d' version.inc
sed -i 's/set(SLIC3R_BUILD_ID "CaribouSlicer-\${SLIC3R_VERSION} Build: \${SLIC3R_BUILD_NR} flavored version of PrusaSlicer (based on SuperSlicer and Slic3r)")/set(SLIC3R_BUILD_ID "PrusaSlicer-\${SLIC3R_VERSION}+UNKNOWN")/' $SCRIPT_PATH/version.inc

sed -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.hpp

sed -i 's/CaribouSlicer_\$/PrusaSlicer_\$/g' $SCRIPT_PATH/CMakeLists.txt

sed -i -e "s/CaribouGcodeviewer_/PrusaSlicer-gcodeviewer_/g" $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed -i -e "s/CaribouGcodeviewer_/PrusaSlicer-gcodeviewer_/g" $SCRIPT_PATH/CMakeLists.txt

sed -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MsgDialog.cpp
sed -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/InstanceCheck.cpp
sed -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.hpp
sed -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/version.inc

sed -i -e 's|URL_CHANGELOG = "https://github.com/caribou3d/CaribouSlicer/releases"|URL_CHANGELOG = "https://files.prusa3d.com/?latest=slicer-stable\&lng=\%1\%"|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp
sed -i -e 's|URL_DOWNLOAD = "https://github.com/caribou3d/CaribouSlicer/releases"|URL_DOWNLOAD = "https://www.prusa3d.com/slicerweb\&lng=\%1\%"|g' src/slic3r/GUI/UpdateDialogs.cpp

sed -i 's|"https://github.com/Caribou3d/CaribouSlicer/blob/master/CaribouSlicer.version?raw=true"|"https://files.prusa3d.com/wp-content/uploads/repository/PrusaSlicer-settings-master/live/PrusaSlicer.version"|g' $SCRIPT_PATH/src/libslic3r/AppConfig.cpp
sed -i 's|"https://caribou3d.com/CaribouSlicer/repository/vendor_indices.zip"|"https://files.prusa3d.com/wp-content/uploads/repository/vendor_indices.zip"|g' $SCRIPT_PATH/src/libslic3r/AppConfig.cpp

sed -i -e 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp
sed -i -e 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.hpp
sed -i -e 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

#sed -n '1h; 1!H; ${ g; s/credits =.*Tools");/creditsreplacement/p }' src/slic3r/GUI/GUI_App.cpp > src/slic3r/GUI/GUI_App.cpp.tmp ; mv src/slic3r/GUI/GUI_App.cpp.tmp src/slic3r/GUI/GUI_App.cpp
#sed -i "
#{/creditsreplacement/ c\
#  \   \         credits = title + \" \" +\\
#                _L(\"is based on Slic3r by Alessandro Ranellucci and the RepRap community.\") + \"\\\n\" +\\
#                _L(\"Developed by Prusa Research.\") + \"\\\n\\\n\" +\\
#                title + \" \" + _L(\"is licensed under the\") + \" \" + _L(\"GNU Affero General Public License, version 3\") + \".\\\n\\\n\" +\\
#                _L(\"Contributions by Vojtech Bubnik, Enrico Turri, Oleksandra Iushchenko, Tamas Meszaros, Lukas Matena, Vojtech Kral, David Kocik and numerous others.\") + \"\\\n\\\n\" +\\
#                _L(\"Artwork model by Creative Tools\");
#};" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

find $SCRIPT_PATH/src/slic3r/GUI -maxdepth 1 -type f ! -name "*branding*" -exec sed -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;
find $SCRIPT_PATH/doc -maxdepth 1 -type f ! -name "*branding*" -exec sed -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;
find $SCRIPT_PATH/src -maxdepth 1 -type f ! -name "*branding*" -exec sed -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;
find $SCRIPT_PATH -maxdepth 1 -type f ! -name "*branding*" -exec sed -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;

find $SCRIPT_PATH/resources/localization -type f -exec sed -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
find $SCRIPT_PATH/src/slic3r/Utils -type f -exec sed -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
find $SCRIPT_PATH/src/platform/msw -type f -exec sed -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
sed -i 's/caribou-slicer.exe/prusa-slicer.exe/g' $SCRIPT_PATH/build_win.bat

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' {} +
find $SCRIPT_PATH/src/slic3r/Utils -type f -exec sed -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' {} +
find $SCRIPT_PATH/src/platform/msw -type f -exec sed -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' {} +
sed -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' $SCRIPT_PATH/build_win.bat
sed -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i 's/\.\/CaribouSlicer/\.\/prusa-slicer/g' "/home/wschadow/gittmp/CaribouSlicer/doc/How to build - Linux et al.md"
sed -i 's/src\/CaribouSlicer/src\/prusa-slicer/g' "/home/wschadow/gittmp/CaribouSlicer/doc/How to build - Mac OS.md"

mv $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
mv $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop
mv $SCRIPT_PATH/src/platform/msw/CaribouSlicer.manifest.in $SCRIPT_PATH/src/platform/msw/PrusaSlicer.manifest.in

sed -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop

sed -i 's/Name=CaribouSlicer/Name=PrusaSlicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
sed -i 's/Icon=CaribouSlicer/Icon=PrusaSlicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
sed -i 's/Exec=CaribouSlicer/Exec=prusa-slicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
sed -i 's/StartupWMClass=CaribouSlicer/StartupWMClass=prusa-slicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop

sed -i 's/Name=Caribou/Name=Prusa/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop
sed -i 's/Exec=CaribouSlicer/Exec=prusa-slicer/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop
sed -i 's/Icon=CaribouSlicer-/Icon=PrusaSlicer-/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop

mv $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
mv $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed -i -e 's/CaribouSlicer.ico/PrusaSlicer.ico/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
sed -i -e 's/CaribouGcodeviewer.ico/PrusaSlicer-gcodeviewer.ico/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed -i -e 's/"Caribou3d Research"/"Prusa Research"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
sed -i -e 's/"CaribouSlicer.manifest"/"PrusaSlicer.manifest"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in

sed -i -e 's/"Caribou3d Research"/"Prusa Research"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in
sed -i -e 's/"CaribouSlicer.manifest"/"PrusaSlicer.manifest"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed -i -e 's/CaribouSlicer.manifest/PrusaSlicer.manifest/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i 's/Copyright \\251 2023 Caribou3d, \\251 2016/Copyright \\251 2016/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
sed -i 's/Copyright \\251 2023 Caribou3d, \\251 2016/Copyright \\251 2016/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed -i -e 's/"CaribouSlicer" : "/"prusa-slicer" : "/g' $SCRIPT_PATH/src/slic3r/Utils/Process.cpp
sed -i -e 's/OUTPUT_NAME "caribou-slicer/OUTPUT_NAME "prusa-slicer/g' $SCRIPT_PATH/src/slic3r/GUI/Mouse3DHandlerMac.mm
sed -i -e 's/StartupWMClass=caribou-slicer/StartupWMClass=prusa-slicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed -i -e 's/caribou-slicer binary/prusa-slicer binary/g' $SCRIPT_PATH/src/CaribouSlicer.cpp
sed -i -e 's/Invoking caribou-slicer/Invoking prusa-slicer/g' $SCRIPT_PATH/src/CaribouSlicer.cpp
sed -i -e 's/Usage: caribouslicer/Usage: prusa-slicer/g' $SCRIPT_PATH/src/CaribouSlicer.cpp

sed -i 's/reate_symlink caribou-slicer/reate_symlink prusa-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed -i 's/NAME "caribou-slicer"/NAME "prusa-slicer"/g' $SCRIPT_PATH/src/CMakeLists.txt
sed -i 's/to caribou-slicer/to prusa-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i 's/ln -sf CaribouSlicer/ln -sf prusa-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i 's/Renaming caribou-slicer, symlinking the G-code viewer to CaribouSlicer"/Symlinking the G-code viewer to PrusaSlicer"/g' "$SCRIPT_PATH/src/CMakeLists.txt"
sed -i 's/"Renaming caribou-slicer, symlinking the G-code viewer to CaribouSlicer, symlinking to prusa-slicer and prusa-gcodeviewer"/"Symlinking the G-code viewer to PrusaSlicer, symlinking to prusa-slicer and prusa-gcodeviewer"/g' "$SCRIPT_PATH/src/CMakeLists.txt"

sed -i 's/COMMAND ln -sf CaribouSlicer caribou-gcodeviewer/COMMAND ln -sf PrusaSlicer caribou-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed -i 's/COMMAND ln -sf CaribouSlicer PrusaGCodeViewer/COMMAND ln -sf PrusaSlicer PrusaGCodeViewer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i '/COMMAND mv caribou-slicer CaribouSlicer/d' $SCRIPT_PATH/src/CMakeLists.txt

sed -i 's/CaribouSlicer.rc/PrusaSlicer.rc/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i '/COMMAND ln -sf prusa-slicer caribou-slicer/d' $SCRIPT_PATH/src/CMakeLists.txt
sed '0,/COMMAND ln -sf prusa-slicer prusa-gcodeviewer/s//COMMAND ln -sf PrusaSlicer prusa-slicer\n            COMMAND ln -sf PrusaSlicer prusa-gcodeviewer\n            COMMAND ln -sf PrusaSlicer PrusaGCodeViewer/' $SCRIPT_PATH/src/CMakeLists.txt > $SCRIPT_PATH/src/CMakeLists.txt.tmp
mv $SCRIPT_PATH/src/CMakeLists.txt.tmp $SCRIPT_PATH/src/CMakeLists.txt

sed -i '/COMMAND ln -sf prusa-slicer CaribouGCodeViewer/d' $SCRIPT_PATH/src/CMakeLists.txt

sed -i '/for various platforms\./,/\(configure_file(\${CMAKE_CURRENT_SOURCE_DIR}\/platform\/msw\/PrusaSlicer.rc.in\)/{//!d;}' "$SCRIPT_PATH/src/CMakeLists.txt"

sed -i -e 's/CaribouSlicer.desktop/PrusaSlicer.desktop/g' $SCRIPT_PATH/CMakeLists.txt
sed -i -e 's/CaribouGcodeviewer.desktop/PrusaGcodeviewer.desktop/g' $SCRIPT_PATH/CMakeLists.txt

sed -i '/resolve_path_from_var("XDG_CONFIG_HOME", target_candidates);/d' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed -i 's/caribouslicer/prusaslicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed -i 's/CaribouSlicer/PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed -i 's/\/\/show_send_system_info_dialog/show_send_system_info_dialog/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed -i 's/CaribouSlicer is based on PrusaSlicer by Prusa Research and SuperSlicer by supermerill. Both are based on Slic3r by Alessandro Ranellucci and the RepRap community.");/PrusaSlicer is based on Slic3r by Alessandro Ranellucci and the RepRap community.");/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp
 
#sed -i '/Copyright \&copy; 2023  Caribou3d Research \& Development. <br \/>/d' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed -i 's/CaribouSlicerG/PrusaSlicerG/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h
sed -i 's/CaribouSlicerG/PrusaSlicerG/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed -i 's/CaribouSlicer.G/PrusaSlicer.G/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/Preferences.cpp
sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h

sed -i 's/Caribou G/Prusa G/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/Config.cpp
sed -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/utils.cpp
sed -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/GCode/GCodeProcessor.cpp
sed -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/GCode/GCodeProcessor.hpp

sed -i 's/caribouslicer_config/prusaslicer_config/g' $SCRIPT_PATH/src/libslic3r/Config.cpp
sed -i 's/caribouslicer_config/prusaslicer_config/g' $SCRIPT_PATH/src/libslic3r/GCode.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed -i 's/CaribouSlicer is released/PrusaSlicer is released/g' {} +
find $SCRIPT_PATH/src/libslic3r -type f -exec sed -i 's/CaribouSlicer is released/PrusaSlicer is released/g' {} +

sed -i 's/string(SLIC3R_VERSION) + " Build" + " " + std::string(SLIC3R_BUILD_NR);/string(SLIC3R_VERSION);/g' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp
 
#sed -i '/#define SLIC3R_BUILD_NR "@SLIC3R_BUILD_NR@/d' $SCRIPT_PATH/src/libslic3r/libslic3r_version.h.in
sed -i 's/string(SLIC3R_VERSION) + " Build" + " " + std::string(SLIC3R_BUILD_NR);/string(SLIC3R_VERSION);/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed -i 's/SLIC3R_BUILD_NR/SLIC3R_BUILD_ID/g' $SCRIPT_PATH/src/slic3r/GUI/SysInfoDialog.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed -i 's/"CaribouSlicer/"PrusaSlicer/g' {} +

sed -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/Utils/OctoPrint.cpp
sed -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/Utils/Http.cpp
sed -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/CaribouSlicer_app_msvc.cpp
sed -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/PrintConfig.cpp
sed -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/Config/Snapshot.cpp

sed -i 's/"caribouslicer/"prusaslicer/g' $SCRIPT_PATH/src/slic3r/GUI/Plater.cpp
sed -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/sandboxes/opencsg/main.cpp

sed -i -e 's/caribouslicer/prusaslicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed -i -e 's/CaribouSlicer/PrusaSlicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/src/CMakeLists.txt

sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/its_neighbor_index/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/meshboolean/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/print_arrange_polys/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/arrange/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/fff_print/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/libslic3r/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/sla_print/CMakeLists.txt
sed -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/slic3rutils/CMakeLists.txt



sed -i -e 's/CaribouSlicer.hpp/PrusaSlicer.hpp/g' $SCRIPT_PATH/src/CaribouSlicer.cpp
mv $SCRIPT_PATH/src/CaribouSlicer.cpp $SCRIPT_PATH/src/PrusaSlicer.cpp
mv $SCRIPT_PATH/src/CaribouSlicer.hpp $SCRIPT_PATH/src/PrusaSlicer.hpp

mv $SCRIPT_PATH/src/CaribouSlicer_app_msvc.cpp $SCRIPT_PATH/src/PrusaSlicer_app_msvc.cpp
