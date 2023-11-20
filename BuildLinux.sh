#!/bin/bash
#
# This script can download and compile dependencies, compile CaribouSlicer
# and optional build a .tgz and an appimage.
#
# Original script from SuperSclier by supermerill https://github.com/supermerill/SuperSlicer
#
# Change log:
#
# 20 Nov 2023, wschadow, branding and minor changes
#

export ROOT=`pwd`
export NCORES=`nproc`
FOUND_GTK2=$(dpkg -l libgtk* | grep gtk2)
FOUND_GTK3=$(dpkg -l libgtk* | grep gtk-3)

unset name
while getopts ":dsiuhgb" opt; do
  case ${opt} in
    u )
        UPDATE_LIB="1"
        ;;
    i )
        BUILD_IMAGE="1"
        ;;
    d )
        BUILD_DEPS="1"
        ;;
    s )
        BUILD_CARIBOUSLICER="1"
        ;;
    b )
        BUILD_DEBUG="1"
        ;;
    g )
        FOUND_GTK3=""
        ;;
    h ) echo "Usage: ./BuildLinux.sh [-i][-u][-d][-s][-b][-g]"
        echo "   -i: Generate appimage (optional)"
        echo "   -g: force gtk2 build"
        echo "   -b: build in debug mode"
        echo "   -d: build deps (optional)"
        echo "   -s: build CaribouSlicer(optional)"
        echo "   -u: only update dependency packets (optional and need sudo)"
        echo "For a first use, you want to 'sudo ./BuildLinux.sh -u'"
        echo "   and then './BuildLinux.sh -dsi'"
        exit 0
        ;;
  esac
done

if [ $OPTIND -eq 1 ]
then
    echo "Usage: ./BuildLinux.sh [-i][-u][-d][-s][-b][-g]"
    echo "   -i: Generate appimage (optional)"
    echo "   -g: force gtk2 build"
    echo "   -b: build in debug mode"
    echo "   -d: build deps (optional)"
    echo "   -s: build CaribouSlicer (optional)"
    echo "   -u: only update dependency packets (optional and need sudo)"
    echo "For a first use, you want to 'sudo ./BuildLinux.sh -u'"
    echo "   and then './BuildLinux.sh -dsi'"
    exit 0
fi

if [[ -n "$UPDATE_LIB" ]]
then
    echo -n -e "Updating linux ...\n"
    apt update
    if [[ -z "$FOUND_GTK3" ]]
    then
        echo -e "\nInstalling: libgtk2.0-dev libglew-dev libudev-dev libdbus-1-dev cmake git\n"
        apt install libgtk2.0-dev libglew-dev libudev-dev libdbus-1-dev cmake git
    else
        echo -e "\nFind libgtk-3, installing: libgtk-3-dev libglew-dev libudev-dev libdbus-1-dev cmake git\n"
        apt install libgtk-3-dev libglew-dev libudev-dev libdbus-1-dev cmake git
    fi
    # for ubuntu 22.04:
    ubu_version="$(cat /etc/issue)" 
    if [[ $ubu_version == "Ubuntu 22.04"* ]]
    then
        apt install curl libssl-dev libcurl4-openssl-dev m4
    fi
    if [[ -n "$BUILD_DEBUG" ]]
    then
        echo -e "\nInstalling: libssl-dev libcurl4-openssl-dev\n"
        apt install libssl-dev libcurl4-openssl-dev
    fi
    echo -e "... done\n"
    exit 0
fi

echo
FOUND_GTK2_DEV=$(dpkg -l libgtk* | grep gtk2.0-dev)
FOUND_GTK3_DEV=$(dpkg -l libgtk* | grep gtk-3-dev)
echo -e "FOUND_GTK2:\n$FOUND_GTK2\n"
echo -e "FOUND_GTK3:\n$FOUND_GTK3)\n"
if [[ -z "$FOUND_GTK2_DEV" ]]
then
if [[ -z "$FOUND_GTK3_DEV" ]]
then
    echo "Error, you must install the dependencies before."
    echo "Use option -u with sudo"
    exit 0
fi
fi

echo "[1/8] Updating submodules..."

# mkdir build
if [ ! -d "build" ]
then
    mkdir build
fi

# mkdir in deps
if [ ! -d "deps/build" ]
then
    mkdir deps/build
fi

if [[ -n "$BUILD_DEPS" ]]
then
    echo "[2/8] Configuring dependencies..."
    BUILD_ARGS=""
    if [[ -n "$FOUND_GTK3_DEV" ]]
    then
        BUILD_ARGS="-DDEP_WX_GTK3=ON"
    fi
    if [[ -n "$BUILD_DEBUG" ]]
    then
        # have to build deps with debug & release or the cmake won't find evrything it needs
        mkdir deps/build/release
        pushd deps/build/release
            cmake ../.. -DDESTDIR="../destdir" $BUILD_ARGS
            make -j$NCORES
        popd
        BUILD_ARGS="${BUILD_ARGS} -DCMAKE_BUILD_TYPE=Debug"
    fi
    
   # cmake deps
    pushd deps/build
        cmake .. $BUILD_ARGS
        echo "done"
        
        # make deps
        echo "[3/8] Building dependencies..."
        make -j$NCORES
        echo "done"
        
        # rename wxscintilla
        echo "[4/8] Renaming wxscintilla library..."
        pushd destdir/usr/local/lib
            if [[ -z "$FOUND_GTK3_DEV" ]]
            then
                cp libwxscintilla-3.1.a libwx_gtk2u_scintilla-3.1.a
            else
                cp libwxscintilla-3.1.a libwx_gtk3u_scintilla-3.1.a
            fi
        popd
        echo "done"
        
        # clean deps
        echo "[5/8] Cleaning dependencies..."
        rm -rf dep_*
    popd
    echo "done"
fi

if [[ -n "$BUILD_CARIBOUSLICER" ]]
then
    echo "[6/8] Configuring CaribouSlicer ..."
    BUILD_ARGS=""
    if [[ -n "$FOUND_GTK3_DEV" ]]
    then
        BUILD_ARGS="-DSLIC3R_GTK=3"
    fi
    if [[ -n "$BUILD_DEBUG" ]]
    then
        BUILD_ARGS="${BUILD_ARGS} -DCMAKE_BUILD_TYPE=Debug"
    fi
    
    # cmake
    pushd build
        cmake .. -DCMAKE_PREFIX_PATH="$PWD/../deps/build/destdir/usr/local" -DSLIC3R_STATIC=1 ${BUILD_ARGS}
        echo "... done"
        
        # make CaribouSlicer
        echo "[7/8] Building CaribouSlicer ..."
        make -j$NCORES

        # make .mo
        make gettext_po_to_mo
    
    popd
    echo "... done"
fi

# Give proper permissions to scripts
chmod 755 $ROOT/build/src/BuildLinuxImage.sh

echo "[8/8] Generating Linux app..."
    pushd build
        if [[ -n "$BUILD_IMAGE" ]]
        then
            $ROOT/build/src/BuildLinuxImage.sh -i
        else
            $ROOT/build/src/BuildLinuxImage.sh
        fi
    popd
echo "done"
  
echo "... done"
