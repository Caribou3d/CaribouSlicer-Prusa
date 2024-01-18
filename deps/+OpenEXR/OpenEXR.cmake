add_cmake_project(Imath
    URL https://github.com/AcademySoftwareFoundation/Imath/archive/refs/tags/v3.1.9.zip
    CMAKE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_TESTING=OFF
)

add_cmake_project(OpenEXR
    URL https://github.com/AcademySoftwareFoundation/openexr/archive/refs/tags/v3.2.1.zip
    URL_HASH SHA256=57d972f94c471a1a4c6aaa25bc6b8609f6b5c1ef466fa8e508d8838795dc4710
    CMAKE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_TESTING=OFF
        -DPYILMBASE_ENABLE:BOOL=OFF
        -DOPENEXR_VIEWERS_ENABLE:BOOL=OFF
        -DOPENEXR_BUILD_UTILS:BOOL=OFF
)

set(DEP_OpenEXR_DEPENDS ZLIB Imath)
