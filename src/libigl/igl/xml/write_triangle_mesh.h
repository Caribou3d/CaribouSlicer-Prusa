// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2016 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef IGL_XML_WRITE_TRIANGLE_MESH_H
#define IGL_XML_WRITE_TRIANGLE_MESH_H
#include "../igl_inline.h"

#include <Eigen/Core>
#include <string>

namespace igl
{
  namespace xml
  {
    // write mesh to a file with automatic detection of file format.  supported:
    // dae, or any of the formats supported by igl::write_triangle_mesh
    //
    // Templates:
    //   Scalar  type for positions and vectors (will be read as double and cast
    //     to Scalar)
    //   Index  type for indices (will be read as int and cast to Index)
    // Inputs:
    //   str  path to file
    //   V  eigen double matrix #V by 3
    //   F  eigen int matrix #F by 3
    // Returns true iff success
    template <typename DerivedV, typename DerivedF>
    IGL_INLINE bool write_triangle_mesh(
      const std::string str,
      const Eigen::PlainObjectBase<DerivedV>& V,
      const Eigen::PlainObjectBase<DerivedF>& F,
      const bool ascii = true);
  }
}

#ifndef IGL_STATIC_LIBRARY
#  include "write_triangle_mesh.cpp"
#endif

#endif

