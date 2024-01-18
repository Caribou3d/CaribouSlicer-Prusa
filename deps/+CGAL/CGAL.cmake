add_cmake_project(
    CGAL
    URL https://github.com/CGAL/cgal/archive/refs/tags/v5.6.zip
    URL_HASH SHA256=109d6316b3bd749208ef5480478b563933dadbdfaaf20edf848814f0baff1ffb
    # URL      https://github.com/CGAL/cgal/archive/refs/tags/v5.4.zip
    # URL_HASH SHA256=d7605e0a5a5ca17da7547592f6f6e4a59430a0bc861948974254d0de43eab4c0
)

include(GNUInstallDirs)

set(DEP_CGAL_DEPENDS Boost GMP MPFR)
