if(BUILD_SHARED_LIBS)
    set(_build_shared ON)
    set(_build_static OFF)
else()
    set(_build_shared OFF)
    set(_build_static ON)
endif()

add_cmake_project(Blosc
     URL https://github.com/Blosc/c-blosc/archive/refs/tags/v1.21.5.zip
     URL_HASH SHA256=bc022fd194e40421531d2ef69831f2793d405d98f60e759c697ccc02dad765ec
    CMAKE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_SHARED=${_build_shared}
        -DBUILD_STATIC=${_build_static}
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DPREFER_EXTERNAL_ZLIB=ON
)

set(DEP_Blosc_DEPENDS ZLIB)
