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

sed $SEDOPTION -i 's/set(SLIC3R_BUILD_ID "PrusaSlicer-\${SLIC3R_VERSION}+UNKNOWN")/set(SLIC3R_BUILD_NR "24034")\nset(SLIC3R_VIEWER "caribou-gcodeviewer")\nset(SLIC3R_BUILD_ID "CaribouSlicer-\${SLIC3R_VERSION} Build: \${SLIC3R_BUILD_NR} flavored version of PrusaSlicer (based on SuperSlicer and Slic3r)")/' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i 's/SLIC3R_VERSION "2.7.0"/SLIC3R_VERSION "2.7.0"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i 's/SLIC3R_RC_VERSION "2,7,0,0"/SLIC3R_RC_VERSION "2,7,0,0"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i 's/SLIC3R_RC_VERSION_DOTS "2.7.0.0"/SLIC3R_RC_VERSION_DOTS "2.7.0.0"/g' $SCRIPT_PATH/version.inc

#=================================

sed $SEDOPTION -i -e 's/"PrusaSlicer"/"CaribouSlicer"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i -e 's/"prusa-slicer"/"CaribouSlicer"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i -e 's/"prusa-slicer"/"CaribouSlicer"/g' $SCRIPT_PATH/version.inc
sed $SEDOPTION -i -e 's/set(SLIC3R_VIEWER_CMD "prusa-gcodeviewer")/set(SLIC3R_VIEWER_CMD "CaribouGcodeviewer")/g' $SCRIPT_PATH/version.inc

#=====================================================================================
#
# modify profiles

find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed $SEDOPTION -i 's/files.prusa3d.com\/wp-content\/uploads\/repository\/PrusaSlicer-settings-master\/live/caribou3d.com\/CaribouSlicer\/repository\/vendors/g' {} +
sed $SEDOPTION -i 's/config_update_url =/config_update_url = https:\/\/caribou3d.com\/CaribouSlicer\/repository\/vendors\/Caribou/' $SCRIPT_PATH/resources/profiles/Caribou.ini

find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed $SEDOPTION -i 's/ PrusaSlicer/ CaribouSlicer/g' {} +

sed $SEDOPTION -i 's#http://files.prusa3d.com/wp-content/uploads/repository/#http://caribou3d.com/CaribouSlicer/repository/#g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp
sed $SEDOPTION -i 's#https://files.prusa3d.com/wp-content/uploads/repository/#https://caribou3d.com/CaribouSlicer/repository/#g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp

sed $SEDOPTION -i 's|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles&lng=%1%|# changelog_url =|g' $SCRIPT_PATH/resources/profiles/*.ini
sed $SEDOPTION -i 's|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles&lng=%1%|# changelog_url =|g' $SCRIPT_PATH/resources/profiles/Sovol/*.ini

#=====================================================================================
#
# modify localization files

find resources/localization -type f -name 'PrusaSlicer.mo' -exec sh -c 'cp "$1" "$(dirname "$1")/CaribouSlicer.mo"' sh {} \;
find resources/localization -type f -name 'PrusaSlicer_*.po' -exec sh -c 'cp "$1" "$(dirname "$1")/CaribouSlicer_$(basename "$1" | cut -d "_" -f 2-)"' sh {} \;
cp $SCRIPT_PATH/resources/localization/PrusaSlicer.pot  $SCRIPT_PATH/resources/localization/CaribouSlicer.pot

sed $SEDOPTION -i 's/PrusaSlicer G-/Caribou G-/g' $SCRIPT_PATH/resources/localization/CaribouSlicer.pot
sed $SEDOPTION -i 's/PrusaSlicer GU/Caribou GU/g' $SCRIPT_PATH/resources/localization/CaribouSlicer.pot
sed $SEDOPTION -i 's/PrusaSlicer/CaribouSlicer/g' $SCRIPT_PATH/resources/localization/CaribouSlicer.pot

find $SCRIPT_PATH/resources/localization -type f -name 'C*.po' -exec sed $SEDOPTION -i 's/prusa-slicer.exe/caribou-slicer.exe/g' {} +
find $SCRIPT_PATH/resources/localization -type f -name 'C*.po' -exec sed $SEDOPTION -i 's/PrusaSlicer/CaribouSlicer/g' {} +
find $SCRIPT_PATH/resources/localization -type f -name 'C*.po' -exec sed $SEDOPTION -i 's/PrusaSlicer GU/CaribouSlicer GU/g' {} +
find $SCRIPT_PATH/resources/localization -type f -name 'C*.po' -exec sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' {} +

sed $SEDOPTION -i 's/PrusaSlicer.pot/CaribouSlicer.pot/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i 's/prusaslicer_copy/caribouslicer_copy/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/PrusaSlicer_app/CaribouSlicer_app/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i 's/PrusaSlicer_\$/CaribouSlicer_\$/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/PrusaSlicer\*.po/CaribouSlicer*.po/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/PrusaSlicer_./CaribouSlicer_./g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i 's/PrusaSlicer.mo/CaribouSlicer.mo/g' $SCRIPT_PATH/CMakeLists.txt

#=====================================================================================
#
# modify icons

sed $SEDOPTION -i -e "s/bf6637/107C18/g" $SCRIPT_PATH/resources/icons/timer_dot.svg
sed $SEDOPTION -i 's/ED8D21/00DD00/g' $SCRIPT_PATH/resources/icons/check_on_focused.svg
sed $SEDOPTION -i 's/ED6B21/00DD00/g' $SCRIPT_PATH/resources/icons/redo.svg $SCRIPT_PATH/resources/icons/undo.svg
sed $SEDOPTION -i 's/00af00/00DD00/g' $SCRIPT_PATH/resources/icons/tick_mark.svg

find $SCRIPT_PATH/resources/icons \( -name 'notification_clippy.svg' -o -name 'PrusaSlicer-gcodeviewer.svg' -o -name 'PrusaSlicer.svg' -o -name 'error_tick_f.svg' -o -name 'error_tick.svg' -o -name 'exclamation.svg' -o -name 'notification_error.svg' -o -name 'notification_warning.svg' \) -prune -o -type f -name '*.svg' -exec sed $SEDOPTION -i 's/ED6B21/107C18/g' {} +
find $SCRIPT_PATH/resources/icons \( -name 'notification_clippy.svg' -o -name 'PrusaSlicer-gcodeviewer.svg' -o -name 'PrusaSlicer.svg' -o -name 'error_tick_f.svg' -o -name 'error_tick.svg' -o -name 'exclamation.svg' -o -name 'notification_error.svg' -o -name 'notification_warning.svg' \) -prune -o -type f -name '*.svg' -exec sed $SEDOPTION -i 's/ed6b21/107c18/g' {} +

sed $SEDOPTION -i 's/ED6B21/ED0000/g' $SCRIPT_PATH/resources/icons/error_tick_f.svg  $SCRIPT_PATH/resources/icons/error_tick.svg $SCRIPT_PATH/resources/icons/exclamation.svg
sed $SEDOPTION -i 's/ed6b21/ed0000/g' $SCRIPT_PATH/resources/icons/notification_error.svg $SCRIPT_PATH/resources/icons/notification_warning.svg

sed $SEDOPTION -i -e 's/PrusaSlicer.png/CaribouSlicer.png/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i -e 's/PrusaSlicer.png/CaribouSlicer.png/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i -e 's/PrusaSlicer-gcodeviewer.png/CaribouGcodeviewer.png/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i -e "s/PrusaSlicer_192/CaribouSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp
sed $SEDOPTION -i -e "s/PrusaSlicer_192/CaribouSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i -e "s/PrusaSlicer_192/CaribouSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/MsgDialog.cpp
sed $SEDOPTION -i -e "s/PrusaSlicer_192/CaribouSlicer_192/g" $SCRIPT_PATH/src/slic3r/GUI/SendSystemInfoDialog.cpp

#=====================================================================================
#
# mainframe

sed $SEDOPTION -i 's/PrusaSlicer")/CaribouSlicer")/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's/PrusaSlicer"/CaribouSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/PrusaSlicer_128/CaribouSlicer_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/PrusaSlicer-mac_128/CaribouSlicer-mac_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/PrusaSlicer-gcodeviewer-mac_128/CaribouGcodeviewer-mac_128/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i "s/PrusaSlicer-gcodeviewer_/CaribouGcodeviewer_/g" $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's|github.com/prusa3d/PrusaSlicer/releases|github.com/caribou3d/CaribouSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's/github.com\/prusa3d\/slic3r\/issues\/new/github.com\/Caribou3d\/CaribouSlicer\/issues\/new\/choose/' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i 's/"PrusaSlicer"/"CaribouSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
sed $SEDOPTION -i '/title += wxString(build_id);/,/SetTitle(title);/c\    title += wxString(build_id);\n    SetTitle(title);' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp

sed $SEDOPTION -i '/wxMenu\* helpMenu = new wxMenu();/,/append_menu_item(helpMenu, wxID_ANY, _L("Software &Releases")/ {/wxMenu\* helpMenu = new wxMenu();/!{/append_menu_item(helpMenu, wxID_ANY, _L("Software &Releases")/!d}}' $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp

#exit
#=====================================================================================
#
# modify colors

sed $SEDOPTION -i -e "s/DARK_ORANGE_PEN   = wxPen(wxColour(237, 107, 33));/DARK_GREEN_PEN   = wxPen(wxColour(16, 124, 24));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/ORANGE_PEN        = wxPen(wxColour(253, 126, 66));/GREEN_PEN        = wxPen(wxColour(27, 204, 40));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/LIGHT_ORANGE_PEN  = wxPen(wxColour(254, 177, 139));/LIGHT_GREEN_PEN   = wxPen(wxColour(114, 237, 123));/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/ORANGE_PEN/GREEN_PEN/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.cpp
sed $SEDOPTION -i -e "s/ORANGE_PEN/GREEN_PEN/g" $SCRIPT_PATH/src/slic3r/GUI/DoubleSlider.hpp

sed $SEDOPTION -i -e "s/ImVec4 orange_color			= ImVec4(.99f, .313f, .0f, 1.0f);/ImVec4 orange_color			= ImVec4(0.0f, 0.859f, 0.031f, 1.0f);/g" $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.cpp

sed $SEDOPTION -i -e "s/wxColour(253, 111, 40) : wxColour(252, 77, 1);/wxColour(24, 180, 36) : wxColour(0, 219, 8);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e "s/wxColour(255, 181, 100): wxColour(203, 61, 0);/wxColour(101, 235, 112): wxColour(1, 140, 13);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e "s/wxColour(95, 73, 62)   : wxColour(228, 220, 216);/wxColour(62, 112, 72)   : wxColour(218, 230, 220);/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i -e "s/ED6B21/1FD12D/g" $SCRIPT_PATH/src/slic3r/GUI/Widgets/UIColors.hpp

sed $SEDOPTION -i -e "s/wxColour(237, 107, 33)/wxColour(16, 124, 24)/g" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i -e "s/ED6B21/107C18/g" $SCRIPT_PATH/src/slic3r/GUI/BitmapCache.cpp
sed $SEDOPTION -i -e "s/ed6b21/107c18/g" $SCRIPT_PATH/src/slic3r/GUI/UnsavedChangesDialog.cpp

sed $SEDOPTION -i 's|static const ColorRGB ORANGE()      { return { 0.92f, 0.50f, 0.26f }; }|static const ColorRGB GREENC()      { return { 0.06f, 0.49f, 0.09f }; }|' $SCRIPT_PATH/src/libslic3r/Color.hpp
sed $SEDOPTION -i 's|static const ColorRGBA ORANGE()      { return { 0.923f, 0.504f, 0.264f, 1.0f }; }|static const ColorRGBA GREENC()      { return { 0.063f, 0.486f, 0.094f, 1.0f }; }|' $SCRIPT_PATH/src/libslic3r/Color.hpp

sed $SEDOPTION -i 's|const ImVec4 ImGuiWrapper::COL_ORANGE_DARK.*|const ImVec4 ImGuiWrapper::COL_GREENC_DARK       = { 0.063f, 0.353f, 0.094f, 1.0f };|; s|const ImVec4 ImGuiWrapper::COL_ORANGE_LIGHT.*|const ImVec4 ImGuiWrapper::COL_GREENC_LIGHT      = to_ImVec4(ColorRGBA::GREENC());|' $SCRIPT_PATH/src/slic3r/GUI/ImGuiWrapper.cpp

sed $SEDOPTION -i -e 's/ORANGE()/GREENC()/g' $SCRIPT_PATH/src/slic3r/GUI/Gizmos/GLGizmoBase.hpp
sed $SEDOPTION -i -e 's/ORANGE()/GREENC()/g' $SCRIPT_PATH/src/slic3r/GUI/Selection.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's|COL_ORANGE_|COL_GREENC_|g' {} +

sed $SEDOPTION -i '/find package\/resources\/localization -name "\*.po" -type f -delete/c\    find package/resources/localization -name "*.po" -type f -delete\n    find package/resources/localization -name "P*.mo" -o -name "*.txt" -o -name "P*.pot" -type f -delete' src/platform/unix/BuildLinuxImage.sh.in

#=====================================================================================

sed $SEDOPTION -i 's/PrusaSlicer GU/CaribouSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed $SEDOPTION -i 's/PrusaSlicer GU/CaribouSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed $SEDOPTION -i 's/PrusaSlicer GU/CaribouSlicer GU/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/slic3r/GUI/Preferences.cpp
sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h

sed $SEDOPTION -i -e 's/PrusaSlicer-gcodeviewer/CaribouGcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/PrusaSlicer-gcodeviewer/CaribouGcodeviewer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i -e 's/PrusaSlicer-gcodeviewer/CaribouGcodeviewer/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.hpp

sed $SEDOPTION -i -e "s/PrusaSlicer-gcodeviewer_/CaribouGcodeviewer_/g" $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i -e "s/PrusaSlicer-gcodeviewer_/CaribouGcodeviewer_/g" $SCRIPT_PATH/CMakeLists.txt

mv $SCRIPT_PATH/src/platform/msw/PrusaSlicer.manifest.in $SCRIPT_PATH/src/platform/msw/CaribouSlicer.manifest.in

mv $SCRIPT_PATH/src/platform/msw/PrusaSlicer.rc.in $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in
mv $SCRIPT_PATH/src/platform/msw/PrusaSlicer-gcodeviewer.rc.in $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in

sed $SEDOPTION -i -e 's/"Prusa Research"/"Caribou3d Research"/g' $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in
sed $SEDOPTION -i -e 's/"PrusaSlicer.manifest"/"CaribouSlicer.manifest"/g' $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in
sed $SEDOPTION -i -e 's/PrusaSlicer.manifest/CaribouSlicer.manifest/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/"PrusaSlicer.manifest"/"CaribouSlicer.manifest"/g' $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in

sed $SEDOPTION -i 's/PrusaSlicer.rc/CaribouSlicer.rc/g' $SCRIPT_PATH/src/CMakeLists.txt

#=====================================================================================

sed $SEDOPTION -i -e 's|URL_CHANGELOG = "https://files.prusa3d.com/?latest=slicer-stable&lng=%1%"|URL_CHANGELOG = "https://github.com/caribou3d/CaribouSlicer/releases"|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp
sed $SEDOPTION -i 's|URL_DOWNLOAD = "https://www.prusa3d.com/slicerweb&lng=%1%"|URL_DOWNLOAD = "https://github.com/caribou3d/CaribouSlicer/releases"|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp

sed $SEDOPTION -i 's|"https://files.prusa3d.com/wp-content/uploads/repository/PrusaSlicer-settings-master/live/PrusaSlicer.version"|"https://github.com/Caribou3d/CaribouSlicer/blob/master/CaribouSlicer.version?raw=true"|g' $SCRIPT_PATH/src/libslic3r/AppConfig.cpp
sed $SEDOPTION -i 's|"https://files.prusa3d.com/wp-content/uploads/repository/vendor_indices.zip"|"https://caribou3d.com/CaribouSlicer/repository/vendor_indices.zip"|g' $SCRIPT_PATH/src/libslic3r/AppConfig.cpp

sed $SEDOPTION -i -e 's|github.com/prusa3d/PrusaSlicer/releases|github.com/caribou3d/CaribouSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/UpdateDialogs.cpp
sed $SEDOPTION -i -e 's|github.com/prusa3d/PrusaSlicer/releases|github.com/caribou3d/CaribouSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/NotificationManager.hpp
sed $SEDOPTION -i -e 's|github.com/prusa3d/PrusaSlicer/releases|github.com/caribou3d/CaribouSlicer/releases|g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

#=====================================================================================

sed $SEDOPTION -i -e 's/"PrusaSlicer"/"CaribouSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/MsgDialog.cpp
sed $SEDOPTION -i -e 's/"PrusaSlicer"/"CaribouSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/InstanceCheck.cpp
sed $SEDOPTION -i -e 's/"PrusaSlicer"/"CaribouSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.hpp

sed $SEDOPTION -i -e 's/g PrusaSlicer /g CaribouSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/o PrusaSlicer /o CaribouSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/n PrusaSlicer /n CaribouSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/t PrusaSlicer /t CaribouSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i -e 's/f PrusaSlicer /f CaribouSlicer /g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -n '1h; 1!H; ${ g; s/credits =.*\\n\\n";/creditsreplacement/p }' src/slic3r/GUI/GUI_App.cpp > src/slic3r/GUI/GUI_App.cpp.tmp ; mv src/slic3r/GUI/GUI_App.cpp.tmp src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i "
{/creditsreplacement/ c\
  \   \         credits = title + \" \" + \\
                _L(\"is based on PrusaSlicer by Prusa Research and SuperSlicer by supermerill.\") + \"\\\n\" + \\
                _L(\"Both are based on Slic3r by Alessandro Ranellucci and the RepRap community.\") + \"\\\n\\\n\" + \\
                title + \" \" + _L(\"is licensed $SEDOPTION under the\") + \" \" + _L(\"GNU Affero General Public License, version 3\") + \".\\\n\\\n\" + \\
                _L(\"Contributions by Vojtech Bubnik, Enrico Turri, Tamas Meszaros, Oleksandra Iushchenko, Lukas Matena, Vojtech Kr#al, David Kocik and numerous others.\") + \"\\\n\\\n\";
};" $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp


sed $SEDOPTION -i -e 's/"PrusaSlicer"/"CaribouSlicer"/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp

#=====================================================================================

find $SCRIPT_PATH/src/slic3r/GUI -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/prusa-slicer-console/caribou-slicer-console/g' {} \;
find $SCRIPT_PATH/doc -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/prusa-slicer-console/caribou-slicer-console/g' {} \;
find $SCRIPT_PATH/src -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/prusa-slicer-console/caribou-slicer-console/g' {} \;
find $SCRIPT_PATH -maxdepth 1 -type f ! -name "*branding*" -exec sed $SEDOPTION -i 's/prusa-slicer-console/caribou-slicer-console/g' {} \;

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/prusa-slicer.exe/caribou-slicer.exe/g' {} +
find $SCRIPT_PATH/src/slic3r/Utils -type f -exec sed $SEDOPTION -i 's/prusa-slicer.exe/caribou-slicer.exe/g' {} +
find $SCRIPT_PATH/src/platform/msw -type f -exec sed $SEDOPTION -i 's/prusa-slicer.exe/caribou-slicer.exe/g' {} +
sed $SEDOPTION -i 's/prusa-slicer.exe/caribou-slicer.exe/g' $SCRIPT_PATH/build_win.bat

#=====================================================================================

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/prusa-gcodeviewer/caribou-gcodeviewer/g' {} +
find $SCRIPT_PATH/src/slic3r/Utils -type f -exec sed $SEDOPTION -i 's/prusa-gcodeviewer/caribou-gcodeviewer/g' {} +
find $SCRIPT_PATH/src/platform/msw -type f -exec sed $SEDOPTION -i 's/prusa-gcodeviewer/caribou-gcodeviewer/g' {} +
sed $SEDOPTION -i 's/prusa-gcodeviewer/caribou-gcodeviewer/g' $SCRIPT_PATH/build_win.bat
sed $SEDOPTION -i 's/prusa-gcodeviewer/caribou-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt

mv $SCRIPT_PATH/src/platform/unix/PrusaSlicer.desktop $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop
mv $SCRIPT_PATH/src/platform/unix/PrusaGcodeviewer.desktop $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop

sed $SEDOPTION -i -e 's/PrusaSlicer-gcodeviewer/CaribouGcodeviewer/g' $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop

sed $SEDOPTION -i 's/Name=PrusaSlicer/Name=CaribouSlicer/g' $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop
sed $SEDOPTION -i 's/Icon=PrusaSlicer/Icon=CaribouSlicer/g' $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop
sed $SEDOPTION -i 's/Exec=prusa-slicer/Exec=CaribouSlicer/g' $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop
sed $SEDOPTION -i 's/StartupWMClass=prusa-slicer/StartupWMClass=CaribouSlicer/g' $SCRIPT_PATH/src/platform/unix/CaribouSlicer.desktop

sed $SEDOPTION -i 's/Name=Prusa/Name=Caribou/g' $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop
sed $SEDOPTION -i 's/Exec=prusa-slicer/Exec=CaribouSlicer/g' $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop
sed $SEDOPTION -i 's/Icon=PrusaSlicer-/Icon=CaribouSlicer-/g' $SCRIPT_PATH/src/platform/unix/CaribouGcodeviewer.desktop

sed $SEDOPTION -i -e 's/PrusaSlicer.ico/CaribouSlicer.ico/g' $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in
sed $SEDOPTION -i -e 's/PrusaSlicer-gcodeviewer.ico/CaribouGcodeviewer.ico/g' $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in

sed $SEDOPTION -i -e 's/"Prusa Research"/"Caribou3d Research"/g' $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in

sed $SEDOPTION -i 's/Copyright \\251 2016/Copyright \\251 2023 Caribou3d, \\251 2016/g' $SCRIPT_PATH/src/platform/msw/CaribouSlicer.rc.in
sed $SEDOPTION -i 's/Copyright \\251 2016/Copyright \\251 2023 Caribou3d, \\251 2016/g' $SCRIPT_PATH/src/platform/msw/CaribouGcodeviewer.rc.in

sed $SEDOPTION -i -e 's/"prusa-slicer" : "/"CaribouSlicer" : "/g' $SCRIPT_PATH/src/slic3r/Utils/Process.cpp
sed $SEDOPTION -i -e 's/OUTPUT_NAME "prusa-slicer/OUTPUT_NAME "caribou-slicer/g' $SCRIPT_PATH/src/slic3r/GUI/Mouse3DHandlerMac.mm
sed $SEDOPTION -i -e 's/StartupWMClass=prusa-slicer/StartupWMClass=caribou-slicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i -e 's/prusa-slicer binary/caribou-slicer binary/g' $SCRIPT_PATH/src/PrusaSlicer.cpp
sed $SEDOPTION -i -e 's/Invoking prusa-slicer/Invoking caribou-slicer/g' $SCRIPT_PATH/src/PrusaSlicer.cpp
sed $SEDOPTION -i -e 's/Usage: prusa-slicer/Usage: caribouslicer/g' $SCRIPT_PATH/src/PrusaSlicer.cpp

sed $SEDOPTION -i 's/reate_symlink prusa-slicer/reate_symlink caribou-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/NAME "prusa-slicer"/NAME "caribou-slicer"/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/to prusa-slicer/to caribou-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i 's/COMMAND ln -sf PrusaSlicer prusa-slicer/COMMAND mv caribou-slicer CaribouSlicer\n            COMMAND ln -sf CaribouSlicer caribou-slicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/ln -sf prusa-slicer/ln -sf CaribouSlicer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i 's/COMMAND ln -sf CaribouSlicer caribou-gcodeviewer/COMMAND mv caribou-slicer CaribouSlicer\n            COMMAND ln -sf CaribouSlicer caribou-slicer\n            COMMAND ln -sf CaribouSlicer CaribouGCodeViewer\n            COMMAND ln -sf CaribouSlicer caribou-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/Symlinking the G-code viewer to PrusaSlicer"/Renaming caribou-slicer, symlinking the G-code viewer to CaribouSlicer"/g' "$SCRIPT_PATH/src/CMakeLists.txt"
sed $SEDOPTION -i 's/"Symlinking the G-code viewer to PrusaSlicer, symlinking to caribou-slicer and caribou-gcodeviewer"/"Renaming caribou-slicer, symlinking the G-code viewer to CaribouSlicer, symlinking to caribou-slicer and caribou-gcodeviewer"/g' "$SCRIPT_PATH/src/CMakeLists.txt"

sed $SEDOPTION -i 's/COMMAND ln -sf PrusaSlicer caribou-gcodeviewer/COMMAND ln -sf CaribouSlicer caribou-gcodeviewer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i 's/COMMAND ln -sf PrusaSlicer PrusaGCodeViewer/COMMAND ln -sf CaribouSlicer CaribouGCodeViewer/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i -e 's/PrusaSlicer.desktop/CaribouSlicer.desktop/g' $SCRIPT_PATH/CMakeLists.txt
sed $SEDOPTION -i -e 's/PrusaGcodeviewer.desktop/CaribouGcodeviewer.desktop/g' $SCRIPT_PATH/CMakeLists.txt

sed $SEDOPTION -i 's/resolve_path_from_var("XDG_DATA_DIRS", target_candidates);/resolve_path_from_var("XDG_DATA_DIRS", target_candidates);\n    resolve_path_from_var("XDG_CONFIG_HOME", target_candidates);/' "$SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp"

sed $SEDOPTION -i 's/prusaslicer/caribouslicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp
sed $SEDOPTION -i 's/PrusaSlicer/CaribouSlicer/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i 's/CaribouSlicer is released/PrusaSlicer is released/g'  $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i 's/show_send_system_info_dialog/\/\/show_send_system_info_dialog/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i 's/PrusaSlicer is based on Slic3r by Alessandro Ranellucci and the RepRap community.");/CaribouSlicer is based on PrusaSlicer by Prusa Research and SuperSlicer by supermerill. Both are based on Slic3r by Alessandro Ranellucci and the RepRap community.");/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i 's/"<font color=%3%>"/"<font color=%3%>"\n            "Copyright \&copy; 2023  Caribou3d Research \& Development. <br \/>"/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i 's/PrusaSlicerG/CaribouSlicerG/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h
sed $SEDOPTION -i 's/PrusaSlicerG/CaribouSlicerG/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp
sed $SEDOPTION -i 's/PrusaSlicer.G/CaribouSlicer.G/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp

sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/slic3r/GUI/Preferences.cpp
sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/slic3r/GUI/GUI_Init.cpp
sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/slic3r/GUI/ConfigWizard.cpp
sed $SEDOPTION -i 's/PrusaSlicer G/Caribou G/g' $SCRIPT_PATH/src/libslic3r/libslic3r.h

sed $SEDOPTION -i 's/Prusa G/Caribou G/g' $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp

sed $SEDOPTION -i 's/generated by PrusaSlicer/generated by CaribouSlicer/g' $SCRIPT_PATH/src/libslic3r/Config.cpp
sed $SEDOPTION -i 's/generated by PrusaSlicer/generated by CaribouSlicer/g' $SCRIPT_PATH/src/libslic3r/utils.cpp
sed $SEDOPTION -i 's/generated by PrusaSlicer/generated by CaribouSlicer/g' $SCRIPT_PATH/src/libslic3r/GCode/GCodeProcessor.cpp
sed $SEDOPTION -i 's/generated by PrusaSlicer/generated by CaribouSlicer/g' $SCRIPT_PATH/src/libslic3r/GCode/GCodeProcessor.hpp

sed $SEDOPTION -i 's/prusaslicer_config/caribouslicer_config/g' $SCRIPT_PATH/src/libslic3r/Config.cpp
sed $SEDOPTION -i 's/prusaslicer_config/caribouslicer_config/g' $SCRIPT_PATH/src/libslic3r/GCode.cpp

sed $SEDOPTION -i 's/string(SLIC3R_VERSION);/string(SLIC3R_VERSION) + " Build" + " " + std::string(SLIC3R_BUILD_NR);/g' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i 's/#define SLIC3R_VERSION "@SLIC3R_VERSION@"/#define SLIC3R_VERSION "@SLIC3R_VERSION@"\n#define SLIC3R_BUILD_NR "@SLIC3R_BUILD_NR@"/' $SCRIPT_PATH/src/libslic3r/libslic3r_version.h.in
sed $SEDOPTION -i 's/string(SLIC3R_VERSION);/string(SLIC3R_VERSION) + " Build" + " " + std::string(SLIC3R_BUILD_NR);/' $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp

sed $SEDOPTION -i 's/SLIC3R_BUILD_ID/SLIC3R_BUILD_NR/g' $SCRIPT_PATH/src/slic3r/GUI/SysInfoDialog.cpp

find $SCRIPT_PATH/src/slic3r/GUI -type f -exec sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' {} +

sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' $SCRIPT_PATH/src/slic3r/Utils/OctoPrint.cpp
sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' $SCRIPT_PATH/src/slic3r/Utils/Http.cpp
sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' $SCRIPT_PATH/src/PrusaSlicer_app_msvc.cpp
sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' $SCRIPT_PATH/src/libslic3r/PrintConfig.cpp
sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' $SCRIPT_PATH/src/slic3r/Config/Snapshot.cpp

sed $SEDOPTION -i 's/"prusaslicer/"caribouslicer/g' $SCRIPT_PATH/src/slic3r/GUI/Plater.cpp
sed $SEDOPTION -i 's/"PrusaSlicer/"CaribouSlicer/g' $SCRIPT_PATH/sandboxes/opencsg/main.cpp

sed $SEDOPTION -i -e 's/prusaslicer/caribouslicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/PrusaSlicer/CaribouSlicer/g' $SCRIPT_PATH/src/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/src/CMakeLists.txt

sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/its_neighbor_index/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/meshboolean/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/sandboxes/print_arrange_polys/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/tests/arrange/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/tests/fff_print/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/tests/libslic3r/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/tests/sla_print/CMakeLists.txt
sed $SEDOPTION -i -e 's/prusaslicer_copy_dll/caribouslicer_copy_dll/g' $SCRIPT_PATH/tests/slic3rutils/CMakeLists.txt

sed $SEDOPTION -i -e 's/PrusaSlicer.hpp/CaribouSlicer.hpp/g' $SCRIPT_PATH/src/PrusaSlicer.cpp

mv $SCRIPT_PATH/src/PrusaSlicer.cpp $SCRIPT_PATH/src/CaribouSlicer.cpp
mv $SCRIPT_PATH/src/PrusaSlicer.hpp $SCRIPT_PATH/src/CaribouSlicer.hpp

mv $SCRIPT_PATH/src/PrusaSlicer_app_msvc.cpp $SCRIPT_PATH/src/CaribouSlicer_app_msvc.cpp

sed $SEDOPTION -i '/Instead of compiling PrusaSlicer from source code, one may also consider to install PrusaSlicer/{N;d;}' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md

sed $SEDOPTION -i 's/PrusaSlicer /CaribouSlicer /g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md
sed $SEDOPTION -i 's/ PrusaSlicer/ CaribouSlicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md
sed $SEDOPTION -i 's/prusa-slicer/caribou-slicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md

sed $SEDOPTION -i 's|https://github.com/prusa3d/PrusaSlicer/releases|https://github.com/caribou3d/CaribouSlicer/releases|g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md
sed $SEDOPTION -i 's|https://www.github.com/prusa3d/PrusaSlicer|https://www.github.com/caribou3d/CaribouSlicer|g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md

sed $SEDOPTION -i 's/multi core/multi-core/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md
sed $SEDOPTION -i 's/fail and/fail, and/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md

sed $SEDOPTION -i 's/PrusaSlicer /CaribouSlicer /g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Mac\ OS.md
sed $SEDOPTION -i 's/ PrusaSlicer/ CaribouSlicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Mac\ OS.md
sed $SEDOPTION -i 's/prusa-slicer/caribou-slicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Mac\ OS.md
sed $SEDOPTION -i 's|https://github.com/prusa3d/PrusaSlicer|https://github.com/caribou3d/CaribouSlicer|g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Mac\ OS.md

sed $SEDOPTION -i 's/PrusaSlicer /CaribouSlicer /g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md
sed $SEDOPTION -i 's/ PrusaSlicer/ CaribouSlicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md
sed $SEDOPTION -i 's/PrusaSlicer_/CaribouSlicer_/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md
sed $SEDOPTION -i 's/src\\PrusaSlicer/src\\CaribouSlicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md
sed $SEDOPTION -i 's/prusa-slicer/caribou-slicer/g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md
sed $SEDOPTION -i 's|https://github.com/prusa3d/PrusaSlicer|https://github.com/caribou3d/CaribouSlicer|g' $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md

if [ $TARGET_OS == "windows" ]; then
   unix2dos -q $SCRIPT_PATH/src/CMakeLists.txt $SCRIPT_PATH/src/libslic3r/libslic3r_version.h.in $SCRIPT_PATH/src/slic3r/GUI/AboutDialog.cpp $SCRIPT_PATH/src/slic3r/GUI/DesktopIntegrationDialog.cpp $SCRIPT_PATH/src/slic3r/GUI/GUI_App.cpp $SCRIPT_PATH/src/slic3r/GUI/ImGuiWrapper.cpp $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
   unix2dos -q $SCRIPT_PATH/doc/How\ to\ build\ -\ Linux\ et\ al.md $SCRIPT_PATH/doc/How\ to\ build\ -\ Mac\ OS.md $SCRIPT_PATH/doc/How\ to\ build\ -\ Windows.md
fi
