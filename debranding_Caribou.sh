#!/bin/bash

if [ -z "$SCRIPT_PATH" ]; then
	SCRIPT_PATH="$( cd "$(dirname "$0")" ; pwd -P )"
fi

OS_FOUND=$( command -v uname)

case $( "${OS_FOUND}" | tr '[:upper:]' '[:lower:]') in
  linux*)
    TARGET_OS="linux"
   ;;
  msys*|cygwin*|mingw*)
    # or possible 'bash on windows'
    TARGET_OS='windows'
   ;;
  nt|win*)
    TARGET_OS='windows'
    ;;
  *)
    TARGET_OS='unknown'
    ;;
esac
# Windows
if [ $TARGET_OS == "windows" ]; then
    if [ $(uname -m) == "x86_64" ]; then
        echo "$(tput setaf 2)Windows 64-bit found$(tput sgr0)"
        Processor="64"
    elif [ $(uname -m) == "i386" ]; then
        echo "$(tput setaf 2)Windows 32-bit found$(tput sgr0)"
        Processor="32"
    else
        echo "$(tput setaf 1)Unsupported OS: Windows $(uname -m)"
        failures 1
    fi
# Linux
elif [ $TARGET_OS == "linux" ]; then
    if [ $(uname -m) == "x86_64" ]; then
        echo "$(tput setaf 2)Linux 64-bit found$(tput sgr0)"
        Processor="64"
    elif [[ $(uname -m) == "i386" || $(uname -m) == "i686" ]]; then
        echo "$(tput setaf 2)Linux 32-bit found$(tput sgr0)"
        Processor="32"
    else
        echo "$(tput setaf 1)Unsupported OS: Linux $(uname -m)"
        failures 1
    fi
else
    #echo "$(tput setaf 1)This script doesn't support your Operating system!"
    #echo "Please use Linux 64-bit or Windows 10 64-bit with Linux subsystem / git-bash"
    #echo "Read the notes of build.sh$(tput sgr0)"
    failures 1
fi

if [ $TARGET_OS == "windows" ]; then
  SEDOPTION=--binary
else
  SEDOPTION=
fi

#=====================================================================================
#
# set version

sed $SEDOPTION -i '/set(SLIC3R_BUILD_NR "24059")/d' version.inc
sed $SEDOPTION -i 's/SLIC3R_VERSION "2.7.0"/SLIC3R_VERSION "2.7.0"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i 's/SLIC3R_RC_VERSION "2,7,0,0"/SLIC3R_RC_VERSION "2,7,0,0"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i 's/SLIC3R_RC_VERSION_DOTS "2.7.0.0"/SLIC3R_RC_VERSION_DOTS "2.7.0.0"/g' $SCRIPT_PATH/version.inc

#=================================

sed $SEDOPTION -i '/set(SLIC3R_VIEWER "caribou-gcodeviewer")/d' version.inc
sed $SEDOPTION -i 's/set(SLIC3R_BUILD_ID "CaribouSlicer-\${SLIC3R_VERSION} Build: \${SLIC3R_BUILD_NR} flavored version of PrusaSlicer (based on SuperSlicer and Slic3r)")/set(SLIC3R_BUILD_ID "PrusaSlicer-\${SLIC3R_VERSION}+UNKNOWN")/' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i -e 's/set(SLIC3R_APP_CMD "CaribouSlicer")/set(SLIC3R_APP_CMD "prusa-slicer")/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i -e 's/set(SLIC3R_VIEWER_CMD "CaribouGcodeviewer")/set(SLIC3R_VIEWER_CMD "prusa-gcodeviewer")/g' $SCRIPT_PATH/version.inc

#=====================================================================================
#
# modify profiles

sed $SEDOPTION -i 's|config_update_url = https://caribou3d.com/CaribouSlicer/repository/vendors/Caribou|config_update_url =|' $SCRIPT_PATH/resources/profiles/Caribou.ini

find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed $SEDOPTION -i 's/caribou3d.com\/CaribouSlicer\/repository\/vendors/files.prusa3d.com\/wp-content\/uploads\/repository\/PrusaSlicer-settings-master\/live/g' {} +
find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed $SEDOPTION -i 's/ CaribouSlicer/ PrusaSlicer/g' {} +

sed $SEDOPTION -i -e 's|"http://caribou3d.com/CaribouSlicer/repository/"|"http://files.prusa3d.com/wp-content/uploads/repository/"|g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp
sed $SEDOPTION -i -e 's|"https://caribou3d.com/CaribouSlicer/repository/"|"https://files.prusa3d.com/wp-content/uploads/repository/"|g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp

#sed $SEDOPTION -i 's|# changelog_url =|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles\&lng=%1%|g' $SCRIPT_PATH/resources/profiles/*.ini
find $SCRIPT_PATH/resources/profiles -type f ! -name "Trimaker.ini" -exec sed $SEDOPTION -i 's|# changelog_url =|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles\&lng=%1%|g' {} +

#=====================================================================================
#
# modify localization files

rm -f $SCRIPT_PATH/resources/localization/CaribouSlicer.pot
find $SCRIPT_PATH/resources/localization -type f -name 'Caribou*' -delete

sed $SEDOPTION -i 's/CaribouSlicer.pot/PrusaSlicer.pot/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i 's/caribouslicer_copy/prusaslicer_copy/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/CaribouSlicer_app/PrusaSlicer_app/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i 's/CaribouSlicer_\$/PrusaSlicer_\$/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/CaribouSlicer\*.po/PrusaSlicer*.po/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/CaribouSlicer_./PrusaSlicer_./g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/CaribouSlicer.mo/PrusaSlicer.mo/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEPOPTION -i 's/Compile Caribou/Compile Prusa/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/Assume Caribou/Assume Prusa/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/project(CaribouSlicer)/project(PrusaSlicer)/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEPOPTION -i 's/CaribouSlicer"/PrusaSlicer"/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/CaribouSlicer is/PrusaSlicer is/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/for Caribou/for Prusa/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/running Caribou/running Prusa/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/with Caribou/with Prusa/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEPOPTION -i 's/CaribouSlicer GUI and the CaribouSlicer/PrusaSlicer GUI and the PrusaSlicer/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i 's/CaribouSlicer/PrusaSlicer/g' $SCRIPT_PATH/CMakePresets.json

#=====================================================================================
#
# modify icons

sed $SEDOPTION -i -e "s/107C18/bf6637/g" $SCRIPT_PATH/resources/icons/timer_dot.svg
sed $SEDOPTION -i 's/00DD00/ED8D21/g' $SCRIPT_PATH/resources/icons/check_on_focused.svg
sed $SEDOPTION -i 's/00DD00/ED6B21/g' $SCRIPT_PATH/resources/icons/redo.svg $SCRIPT_PATH/resources/icons/undo.svg
sed $SEDOPTION -i 's/00DD00/00af00/g' $SCRIPT_PATH/resources/icons/tick_mark.svg

find $SCRIPT_PATH/resources/icons \( -name 'notification_clippy.svg' -o -name 'CaribouGcodeviewer.svg' -o -name 'PrusaSlicer.svg' -o -name 'error_tick_f.svg' -o -name 'error_tick.svg' -o -name 'exclamation.svg' -o -name 'notification_error.svg' -o -name 'notification_warning.svg' \) -prune -o -type f -name '*.svg' -exec sed $SEDOPTION -i 's/107C18/ED6B21/g' {} +
find $SCRIPT_PATH/resources/icons \( -name 'notification_clippy.svg' -o -name 'CaribouGcodeviewer.svg' -o -name 'PrusaSlicer.svg' -o -name 'error_tick_f.svg' -o -name 'error_tick.svg' -o -name 'exclamation.svg' -o -name 'notification_error.svg' -o -name 'notification_warning.svg' \) -prune -o -type f -name '*.svg' -exec sed $SEDOPTION -i 's/107c18/ed6b21/g' {} +

sed $SEDOPTION -i 's/ED0000/ED6B21/g' $SCRIPT_PATH/resources/icons/error_tick_f.svg  $SCRIPT_PATH/resources/icons/error_tick.svg $SCRIPT_PATH/resources/icons/exclamation.svg
sed $SEDOPTION -i 's/ed0000/ed6b21/g' $SCRIPT_PATH/resources/icons/notification_error.svg $SCRIPT_PATH/resources/icons/notification_warning.svg

sed $SEDOPTION -i -e 's/CaribouSlicer.png/PrusaSlicer.png/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i -e 's/CaribouSlicer.png/PrusaSlicer.png/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i -e 's/CaribouGcodeviewer.png/PrusaSlicer-gcodeviewer.png/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp
sed $SEDOPTION -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/MsgDialog.cpp
sed $SEDOPTION -i -e "s/CaribouSlicer_192/PrusaSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/SendSystemInfoDialog.cpp

#=====================================================================================
#
# mainframe

sed $SEDOPTION -i 's/CaribouSlicer")/PrusaSlicer")/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/CaribouSlicer_128/PrusaSlicer_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/CaribouSlicer-mac_128/PrusaSlicer-mac_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/CaribouGcodeviewer-mac_128/PrusaSlicer-gcodeviewer-mac_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/CaribouGcodeviewer_/PrusaSlicer-gcodeviewer_/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's/github.com\/Caribou3d\/CaribouSlicer\/issues\/new\/choose/github.com\/prusa3d\/slic3r\/issues\/new/' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's/CaribouSlicer"/PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i '/title += wxString(build_id);/a\    if (wxGetApp().is_editor())\n        title += (" " + _L("based on Slic3r"));\n' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp

sed $SEDOPTION -i '/wxMenu\* helpMenu = new wxMenu();/a\
\
    append_menu_item(helpMenu, wxID_ANY, wxString::Format(_L("%s &Website"), SLIC3R_APP_NAME),\
        wxString::Format(_L("Open the %s website in your browser"), SLIC3R_APP_NAME),\
        [](wxCommandEvent&) { wxGetApp().open_web_page_localized("https://www.prusa3d.com/slicerweb"); });\
    // TRN Item from "Help" menu\
    append_menu_item(helpMenu, wxID_ANY, wxString::Format(_L("&Quick Start"), SLIC3R_APP_NAME),\
        wxString::Format(_L("Open the %s website in your browser"), SLIC3R_APP_NAME),\
        [](wxCommandEvent&) { wxGetApp().open_browser_with_warning_dialog("https://help.prusa3d.com/article/first-print-with-prusaslicer_1753", nullptr, false); });\
    // TRN Item from "Help" menu\
    append_menu_item(helpMenu, wxID_ANY, wxString::Format(_L("Sample &G-codes and Models"), SLIC3R_APP_NAME),\
        wxString::Format(_L("Open the %s website in your browser"), SLIC3R_APP_NAME),\
        [](wxCommandEvent&) { wxGetApp().open_browser_with_warning_dialog("https://help.prusa3d.com/article/sample-g-codes_529630", nullptr, false); });\
    helpMenu->AppendSeparator();\
    append_menu_item(helpMenu, wxID_ANY, _L("Prusa 3D &Drivers"), _L("Open the Prusa3D drivers download page in your browser"),\
        [](wxCommandEvent&) { wxGetApp().open_web_page_localized("https://www.prusa3d.com/downloads"); });' src/slic3r/GUI/MainFrame.cpp

#=====================================================================================
#
# colors

sed $SEDOPTION -i -e "s/DARK_GREEN_PEN   = wxPen(wxColour(16, 124, 24));/DARK_ORANGE_PEN   = wxPen(wxColour(237, 107, 33));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/GREEN_PEN        = wxPen(wxColour(27, 204, 40));/ORANGE_PEN        = wxPen(wxColour(253, 126, 66));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/LIGHT_GREEN_PEN   = wxPen(wxColour(114, 237, 123));/LIGHT_ORANGE_PEN  = wxPen(wxColour(254, 177, 139));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/GREEN_PEN/ORANGE_PEN/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/GREEN_PEN/ORANGE_PEN/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.hpp

sed $SEDOPTION -i -e "s/ImVec4 orange_color			= ImVec4(0.0f, 0.859f, 0.031f, 1.0f);/ImVec4 orange_color			= ImVec4(.99f, .313f, .0f, 1.0f);/g" $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.cpp

sed $SEDOPTION -i -e "s/wxColour(24, 180, 36) : wxColour(0, 219, 8);/wxColour(253, 111, 40) : wxColour(252, 77, 1);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e "s/wxColour(101, 235, 112): wxColour(1, 140, 13);/wxColour(255, 181, 100): wxColour(203, 61, 0);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e "s/wxColour(62, 112, 72)   : wxColour(218, 230, 220);/wxColour(95, 73, 62)   : wxColour(228, 220, 216);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i -e "s/1FD12D/ED6B21/g" $SCRIPT_PATH/src/slic3r/GUI/Widgets/UIColors.hpp

sed $SEDOPTION -i -e "s/wxColour(16, 124, 24)/wxColour(237, 107, 33)/g"  $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i -e "s/107C18/ED6B21/g" $SCRIPT_PATH/src/slic3r/GUI/BitmapCache.cpp
sed $SEDOPTION -i -e "s/107c18/ed6b21/g" $SCRIPT_PATH/src/slic3r/GUI/UnsavedChangesDialog.cpp

sed $SEDOPTION -i 's|static const ColorRGB GREENC()      { return { 0.06f, 0.49f, 0.09f }; }|static const ColorRGB ORANGE()      { return { 0.92f, 0.50f, 0.26f }; }|' $SCRIPT_PATH/src/libslic3r/Color.hpp
sed $SEDOPTION -i 's|static const ColorRGBA GREENC()      { return { 0.063f, 0.486f, 0.094f, 1.0f }; }|static const ColorRGBA ORANGE()      { return { 0.923f, 0.504f, 0.264f, 1.0f }; }|' $SCRIPT_PATH/src/libslic3r/Color.hpp

sed $SEDOPTION -i 's|const ImVec4 ImGuiWrapper::COL_GREENC_DARK.*|const ImVec4 ImGuiWrapper::COL_ORANGE_DARK       = { 0.67f, 0.36f, 0.19f, 1.0f };|; s|const ImVec4 ImGuiWrapper::COL_GREENC_LIGHT.*|const ImVec4 ImGuiWrapper::COL_ORANGE_LIGHT      = to_ImVec4(ColorRGBA::ORANGE());|' $SCRIPT_PATH/src/slic3r/GUI/ImGuiWrapper.cpp

sed $SEDOPTION -i -e 's/GREENC()/ORANGE()/g' $SCRIPT_PATH/src/slic3r/GUI/Gizmos/GLGizmoBase.hpp
sed $SEDOPTION -i -e 's/GREENC()/ORANGE()/g' $SCRIPT_PATH/src/slic3r/GUI/Selection.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's|COL_GREENC_|COL_ORANGE_|g' {} +

sed $SEDOPTION -i '/find package\/resources\/localization -name "P\*.mo" -o -name "\*.txt" -o -name "P\*.pot" -type f -delete/d' src/platform/unix/BuildLinuxImage.sh.in

#=====================================================================================

sed $SEDOPTION -i 's/CaribouSlicer GU/PrusaSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed $SEDOPTION -i 's/CaribouSlicer GU/PrusaSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed $SEDOPTION -i 's/CaribouSlicer GU/PrusaSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/Preferences.cpp
sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h

sed $SEDOPTION -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.hpp

sed $SEDOPTION -i 's/CaribouSlicer_\$/PrusaSlicer_\$/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i -e "s/CaribouGcodeviewer_/PrusaSlicer-gcodeviewer_/g" $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i -e "s/CaribouGcodeviewer_/PrusaSlicer-gcodeviewer_/g" $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MsgDialog.cpp
sed $SEDOPTION -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/InstanceCheck.cpp
sed $SEDOPTION -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.hpp
sed $SEDOPTION -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/"CaribouSlicer"/"PrusaSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp

sed $SEDOPTION -i -e 's/g CaribouSlicer /g PrusaSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/o CaribouSlicer /o PrusaSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/n CaribouSlicer /n PrusaSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/t CaribouSlicer /t PrusaSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/f CaribouSlicer /f PrusaSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i -e 's|URL_CHANGELOG = "https://github.com/caribou3d/CaribouSlicer/releases"|URL_CHANGELOG = "https://files.prusa3d.com/?latest=slicer-stable\&lng=\%1\%"|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp
sed $SEDOPTION -i -e 's|URL_DOWNLOAD = "https://github.com/caribou3d/CaribouSlicer/releases"|URL_DOWNLOAD = "https://www.prusa3d.com/slicerweb\&lng=\%1\%"|g' src/slic3r/GUI/UpdateDialogs.cpp

sed $SEDOPTION -i 's|"https://github.com/Caribou3d/CaribouSlicer/blob/master/CaribouSlicer.version?raw=true"|"https://files.prusa3d.com/wp-content/uploads/repository/PrusaSlicer-settings-master/live/PrusaSlicer.version"|g' $SCRIPT_PATH/src/libslic3r/AppConfig.cpp
sed $SEDOPTION -i 's|"https://caribou3d.com/CaribouSlicer/repository/vendor_indices.zip"|"https://files.prusa3d.com/wp-content/uploads/repository/vendor_indices.zip"|g' $SCRIPT_PATH/src/libslic3r/AppConfig.cpp

sed $SEDOPTION -i -e 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp
sed $SEDOPTION -i -e 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.hpp
sed $SEDOPTION -i -e 's|github.com/caribou3d/CaribouSlicer/releases|github.com/prusa3d/PrusaSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -n '1h; 1!H; ${ g; s/credits =.*\\n\\n";/creditsreplacement/p }' src/slic3r/GUI/GUI_App.cpp > src/slic3r/GUI/GUI_App.cpp.tmp ; mv src/slic3r/GUI/GUI_App.cpp.tmp src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i "
{/creditsreplacement/ c\
  \   \         credits = \"\\\n\" + title + \" \" +\\
                _L(\"is based on Slic3r by Alessandro Ranellucci and the RepRap community.\") + \"\\\n\\\n\" +\\
                _L(\"Developed by Prusa Research.\") + \"\\\n\\\n\" +\\
                _L(\"Licensed under GNU AGPLv3.\") + \"\\\n\\\n\\\n\\\n\\\n\\\n\\\n\";
};" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

find $SCRIPT_PATH/src/slic3r/GUI -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;
find $SCRIPT_PATH/src -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;
find $SCRIPT_PATH -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/caribou-slicer-console/prusa-slicer-console/g' {} \;

find $SCRIPT_PATH/resources/localization -type f -exec sed $SEDOPTION -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
find $SCRIPT_PATH/src/slic3r/Utils -type f -exec sed $SEDOPTION -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
find $SCRIPT_PATH/src/platform/msw -type f -exec sed $SEDOPTION -i 's/caribou-slicer.exe/prusa-slicer.exe/g' {} +
sed $SEDOPTION -i 's/caribou-slicer.exe/prusa-slicer.exe/g' $SCRIPT_PATH/build_win.bat

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' {} +
find $SCRIPT_PATH/src/slic3r/Utils -type f -exec sed $SEDOPTION -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' {} +
find $SCRIPT_PATH/src/platform/msw -type f -exec sed $SEDOPTION -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' {} +
sed $SEDOPTION -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' $SCRIPT_PATH/build_win.bat
sed $SEDOPTION -i 's/caribou-gcodeviewer/prusa-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt

mv $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
mv $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop
mv $SCRIPT_PATH/src/platform/msw/CaribouSlicer.manifest.in $SCRIPT_PATH/src/platform/msw/PrusaSlicer.manifest.in

sed $SEDOPTION -i -e 's/CaribouGcodeviewer/PrusaSlicer-gcodeviewer/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop

sed $SEDOPTION -i 's/Name=CaribouSlicer/Name=PrusaSlicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
sed $SEDOPTION -i 's/Icon=CaribouSlicer/Icon=PrusaSlicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
sed $SEDOPTION -i 's/Exec=CaribouSlicer/Exec=prusa-slicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop
sed $SEDOPTION -i 's/StartupWMClass=CaribouSlicer/StartupWMClass=prusa-slicer/g' $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop

sed $SEDOPTION -i 's/Name=Caribou/Name=Prusa/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop
sed $SEDOPTION -i 's/Exec=CaribouSlicer/Exec=prusa-slicer/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop
sed $SEDOPTION -i 's/Icon=CaribouSlicer-/Icon=PrusaSlicer-/g' $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop

mv $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
mv $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed $SEDOPTION -i -e 's/CaribouSlicer.ico/PrusaSlicer.ico/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
sed $SEDOPTION -i -e 's/CaribouGcodeviewer.ico/PrusaSlicer-gcodeviewer.ico/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed $SEDOPTION -i -e 's/"Caribou3d Research"/"Prusa Research"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
sed $SEDOPTION -i -e 's/"CaribouSlicer.manifest"/"PrusaSlicer.manifest"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in

sed $SEDOPTION -i -e 's/"Caribou3d Research"/"Prusa Research"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in
sed $SEDOPTION -i -e 's/"CaribouSlicer.manifest"/"PrusaSlicer.manifest"/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed $SEDOPTION -i -e 's/CaribouSlicer.manifest/PrusaSlicer.manifest/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i 's/Copyright \\251 2023 Caribou3d, \\251 2016/Copyright \\251 2016/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in
sed $SEDOPTION -i 's/Copyright \\251 2023 Caribou3d, \\251 2016/Copyright \\251 2016/g' $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in

sed $SEDOPTION -i -e 's/"CaribouSlicer" : "/"prusa-slicer" : "/g' $SCRIPT_PATH/src/slic3r/Utils/Process.cpp
sed $SEDOPTION -i -e 's/OUTPUT_NAME "caribou-slicer/OUTPUT_NAME "prusa-slicer/g' $SCRIPT_PATH/src/slic3r/GUI/Mouse3DHandlerMac.mm
sed $SEDOPTION -i -e 's/StartupWMClass=caribou-slicer/StartupWMClass=prusa-slicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i -e 's/caribou-slicer binary/prusa-slicer binary/g' $SCRIPT_PATH/src/CaribouSlicer.cpp
sed $SEDOPTION -i -e 's/Invoking caribou-slicer/Invoking prusa-slicer/g' $SCRIPT_PATH/src/CaribouSlicer.cpp
sed $SEDOPTION -i -e 's/Usage: caribouslicer/Usage: prusa-slicer/g' $SCRIPT_PATH/src/CaribouSlicer.cpp

sed $SEDOPTION -i 's/reate_symlink caribou-slicer/reate_symlink prusa-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/NAME "caribou-slicer"/NAME "prusa-slicer"/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/to caribou-slicer/to prusa-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i 's/ln -sf CaribouSlicer/ln -sf prusa-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i 's/Renaming caribou-slicer, symlinking the G-code viewer to CaribouSlicer"/Symlinking the G-code viewer to PrusaSlicer"/g' "$SCRIPT_PATH/src/CMakeLists.txt"
sed $SEDOPTION -i 's/"Renaming caribou-slicer, symlinking the G-code viewer to CaribouSlicer, symlinking to prusa-slicer and prusa-gcodeviewer"/"Symlinking the G-code viewer to PrusaSlicer, symlinking to prusa-slicer and prusa-gcodeviewer"/g' "$SCRIPT_PATH/src/CMakeLists.txt"

sed $SEDOPTION -i 's/COMMAND ln -sf CaribouSlicer caribou-gcodeviewer/COMMAND ln -sf PrusaSlicer caribou-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/COMMAND ln -sf CaribouSlicer PrusaGCodeViewer/COMMAND ln -sf PrusaSlicer PrusaGCodeViewer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i '/COMMAND mv caribou-slicer CaribouSlicer/d' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/CaribouSlicer.rc/PrusaSlicer.rc/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i '/COMMAND ln -sf prusa-slicer caribou-slicer/d' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION '0,/COMMAND ln -sf prusa-slicer prusa-gcodeviewer/s//COMMAND ln -sf PrusaSlicer prusa-slicer\n            COMMAND ln -sf PrusaSlicer prusa-gcodeviewer\n            COMMAND ln -sf PrusaSlicer PrusaGCodeViewer/' $SCRIPT_PATH/src/CMakeLists.txt > $SCRIPT_PATH/src/CMakeLists.txt.tmp
mv $SCRIPT_PATH/src/CMakeLists.txt.tmp $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i '/COMMAND ln -sf prusa-slicer CaribouGCodeViewer/d' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i -e 's/CaribouSlicer.desktop/PrusaSlicer.desktop/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i -e 's/CaribouGcodeviewer.desktop/PrusaGcodeviewer.desktop/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i '/resolve_path_from_var("XDG_CONFIG_HOME", target_candidates);/d' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i 's/caribouslicer/prusaslicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i 's/CaribouSlicer/PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i 's/\/\/show_send_system_info_dialog/show_send_system_info_dialog/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i 's/CaribouSlicer is based on PrusaSlicer by Prusa Research and SuperSlicer by supermerill. Both are based on Slic3r by Alessandro Ranellucci and the RepRap community.");/PrusaSlicer is based on Slic3r by Alessandro Ranellucci and the RepRap community.");/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i '/Copyright \&copy; 2023  Caribou3d Research \& Development. <br \/>/d' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i 's/CaribouSlicerG/PrusaSlicerG/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h
sed $SEDOPTION -i 's/CaribouSlicerG/PrusaSlicerG/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i 's/CaribouSlicer.G/PrusaSlicer.G/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/Preferences.cpp
sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i 's/Caribou G/PrusaSlicer G/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h

sed $SEDOPTION -i 's/Caribou G/Prusa G/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/Config.cpp
sed $SEDOPTION -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/utils.cpp
sed $SEDOPTION -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/GCode/GCodeProcessor.cpp
sed $SEDOPTION -i 's/generated by CaribouSlicer/generated by PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/GCode/GCodeProcessor.hpp

sed $SEDOPTION -i 's/caribouslicer_config/prusaslicer_config/g' $SCRIPT_PATH/src/libslic3r/Config.cpp
sed $SEDOPTION -i 's/caribouslicer_config/prusaslicer_config/g' $SCRIPT_PATH/src/libslic3r/GCode.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/CaribouSlicer is released/PrusaSlicer is released/g' {} +
find $SCRIPT_PATH/src/libslic3r -type f -exec sed $SEDOPTION -i 's/CaribouSlicer is released/PrusaSlicer is released/g' {} +

sed $SEDOPTION -i 's/string(SLIC3R_VERSION) + " Build" + " " + std::string(SLIC3R_BUILD_NR);/string(SLIC3R_VERSION);/g' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i '/#define SLIC3R_BUILD_NR "@SLIC3R_BUILD_NR@/d' $SCRIPT_PATH/src/libslic3r/libslic3r_version.h.in
sed $SEDOPTION -i 's/string(SLIC3R_VERSION) + " Build" + " " + std::string(SLIC3R_BUILD_NR);/string(SLIC3R_VERSION);/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i 's/SLIC3R_BUILD_NR/SLIC3R_BUILD_ID/g' $SCRIPT_PATH/src/slic3r/GUI/SysInfoDialog.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' {} +

sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/Utils/OctoPrint.cpp
sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/Utils/Http.cpp
sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/CaribouSlicer_app_msvc.cpp
sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/libslic3r/PrintConfig.cpp
sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/src/slic3r/Config/Snapshot.cpp

sed $SEDOPTION -i 's/"caribouslicer/"prusaslicer/g' $SCRIPT_PATH/src/slic3r/GUI/Plater.cpp
sed $SEDOPTION -i 's/"CaribouSlicer/"PrusaSlicer/g' $SCRIPT_PATH/sandboxes/opencsg/main.cpp

sed $SEDOPTION -i -e 's/caribouslicer/prusaslicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/CaribouSlicer/PrusaSlicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/its_neighbor_index/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/meshboolean/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/print_arrange_polys/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/arrange/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/fff_print/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/libslic3r/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/sla_print/CMakeLists.txt
sed $SEDOPTION -i -e 's/caribouslicer_copy_dll/prusaslicer_copy_dll/g' $SCRIPT_PATH/tests/slic3rutils/CMakeLists.txt

sed $SEDOPTION -i -e 's/CaribouSlicer.hpp/PrusaSlicer.hpp/g' $SCRIPT_PATH/src/CaribouSlicer.cpp
mv $SCRIPT_PATH/src/CaribouSlicer.cpp $SCRIPT_PATH/src/PrusaSlicer.cpp
mv $SCRIPT_PATH/src/CaribouSlicer.hpp $SCRIPT_PATH/src/PrusaSlicer.hpp

mv $SCRIPT_PATH/src/CaribouSlicer_app_msvc.cpp $SCRIPT_PATH/src/PrusaSlicer_app_msvc.cpp

if [ $TARGET_OS == "windows" ]; then
   unix2dos -q $SCRIPT_PATH/src/CMakeLists.txt $SCRIPT_PATH/src/libslic3r/libslic3r_version.h.in $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp $SCRIPT_PATH/src/slic3r/GUI/ImGuiWrapper.cpp $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
fi
