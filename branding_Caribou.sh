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
# modify profiles

find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed $SEDOPTION -i 's/files.prusa3d.com\/wp-content\/uploads\/repository\/PrusaSlicer-settings-master\/live/caribou3d.com\/CaribouSlicer\/repository\/vendors/g' {} +
find $SCRIPT_PATH/resources/profiles -type f -name '*.ini' -exec sed $SEDOPTION -i 's/ PrusaSlicer/ CaribouSlicer/g' {} +
 
sed $SEDOPTION -i 's#http://files.prusa3d.com/wp-content/uploads/repository/#http://caribou3d.com/CaribouSlicer/repository/#g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp
sed $SEDOPTION -i 's#https://files.prusa3d.com/wp-content/uploads/repository/#https://caribou3d.com/CaribouSlicer/repository/#g' $SCRIPT_PATH/src/slic3r/Utils/PresetUpdater.cpp

sed $SEDOPTION -i 's|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles&lng=%1%|# changelog_url =|g' $SCRIPT_PATH/resources/profiles/*.ini
sed $SEDOPTION -i 's|# changelog_url = https://files.prusa3d.com/?latest=slicer-profiles&lng=%1%|# changelog_url =|g' $SCRIPT_PATH/resources/profiles/Sovol/*.ini


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

sed $SEDOPTION -i 's/1.00f, 0.49f, 0.22f, 1.0f/0.00f, 1.00f, 0.01f, 1.0f/g' $SCRIPT_PATH/src/slic3r/GUI/GCodeViewer.cpp
sed $SEDOPTION -i 's/0.00f, 1.00f, 0.00f, 1.0f/1.00f, 0.49f, 0.21f, 1.0f/g' $SCRIPT_PATH/src/slic3r/GUI/GCodeViewer.cpp

#=====================================================================================

if [ $TARGET_OS == "windows" ]; then
   unix2dos -q $SCRIPT_PATH/src/CMakeLists.txt $SCRIPT_PATH/src/libslic3r/libslic3r_version.h.in  $SCRIPT_PATH/src/slic3r/GUI/ImGuiWrapper.cpp $SCRIPT_PATH/src/slic3r/GUI/MainFrame.cpp
fi
