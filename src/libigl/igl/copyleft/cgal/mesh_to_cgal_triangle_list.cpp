// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2014 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "mesh_to_cgal_triangle_list.h"
#include "assign.h"

#include <cassert>

template <
  typename DerivedV,
  typename DerivedF,
  typename Kernel>
IGL_INLINE void igl::copyleft::cgal::mesh_to_cgal_triangle_list(
  const Eigen::MatrixBase<DerivedV> & V,
  const Eigen::MatrixBase<DerivedF> & F,
  std::vector<CGAL::Triangle_3<Kernel> > & T)
{
  typedef CGAL::Point_3<Kernel>    Point_3;
  typedef CGAL::Triangle_3<Kernel> Triangle_3;
  // Must be 3D
  assert(V.cols() == 3);
  // **Copy** to convert to output type (this is especially/only needed if the
  // input type DerivedV::Scalar is CGAL::Epeck
  Eigen::Matrix<
    typename Kernel::FT,
    DerivedV::RowsAtCompileTime,
    DerivedV::ColsAtCompileTime>
    KV(V.rows(),V.cols());
  assign(V,KV);
  // Must be triangles
  assert(F.cols() == 3);
  T.reserve(F.rows());
  // Loop over faces
  for(int f = 0;f<(int)F.rows();f++)
  {
    T.push_back(
      Triangle_3(
        Point_3( KV(F(f,0),0), KV(F(f,0),1), KV(F(f,0),2)),
        Point_3( KV(F(f,1),0), KV(F(f,1),1), KV(F(f,1),2)),
        Point_3( KV(F(f,2),0), KV(F(f,2),1), KV(F(f,2),2))));
  }
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, -1, 1, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, -1, 1, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, -1, 1, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, -1, 1, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, -1, 1, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 1, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, -1, 1, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 1, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
// generated by autoexplicit.sh
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, 8, 3, 0, 8, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epick>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, 8, 3, 0, 8, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epick>, std::allocator<CGAL::Triangle_3<CGAL::Epick> > >&);
template void igl::copyleft::cgal::mesh_to_cgal_triangle_list<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, 8, 3, 0, 8, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, CGAL::Epeck>(Eigen::MatrixBase<Eigen::Matrix<CGAL::Lazy_exact_nt<CGAL::Gmpq>, 8, 3, 0, 8, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, std::vector<CGAL::Triangle_3<CGAL::Epeck>, std::allocator<CGAL::Triangle_3<CGAL::Epeck> > >&);
#endif
