#include "psr/poisson_surface_reconstruction.h"
#include <igl/copyleft/marching_cubes.h>
#include <algorithm>
#include <iostream>
#include "geo_utils/fd_interpolate.h"
#include "geo_utils/fd_partial_derivative.h"
#include "geo_utils/fd_grad.h"
#include "igl/cat.h" 

void poisson_surface_reconstruction(
    const Eigen::MatrixXd & P,
    const Eigen::MatrixXd & N,
    Eigen::MatrixXd & V,
    Eigen::MatrixXi & F)
{
  // number of input points
  const int n = P.rows();
  // Grid dimensions
  int nx, ny, nz;
  // Maximum extent (side length of bounding box) of points
  double max_extent =
    (P.colwise().maxCoeff()-P.colwise().minCoeff()).maxCoeff();
  // padding: number of cells beyond bounding box of input points
  const double pad = 8;
  // choose grid spacing (h) so that shortest side gets 30+2*pad samples
  double h  = max_extent/double(30+2*pad);
  // Place bottom-left-front corner of grid at minimum of points minus padding
  Eigen::RowVector3d corner = P.colwise().minCoeff().array()-pad*h;
  // Grid dimensions should be at least 3 
  nx = std::max((P.col(0).maxCoeff()-P.col(0).minCoeff()+(2.*pad)*h)/h,3.);
  ny = std::max((P.col(1).maxCoeff()-P.col(1).minCoeff()+(2.*pad)*h)/h,3.);
  nz = std::max((P.col(2).maxCoeff()-P.col(2).minCoeff()+(2.*pad)*h)/h,3.);
  // Compute positions of grid nodes
  Eigen::MatrixXd x(nx*ny*nz, 3);
  for(int i = 0; i < nx; i++) 
  {
    for(int j = 0; j < ny; j++)
    {
      for(int k = 0; k < nz; k++)
      {
         // Convert subscript to index
         const auto ind = i + nx*(j + k * ny);
         x.row(ind) = corner + h*Eigen::RowVector3d(i,j,k);
      }
    }
  }

  Eigen::VectorXd g = Eigen::VectorXd::Zero(nx*ny*nz);

  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////

  // Interpolation matrix for normals
  Eigen::SparseMatrix<double> Wx(P.rows(), (nx-1)*ny*nz); //, Wy, Wz;
  Eigen::SparseMatrix<double> Wy(P.rows(), nx*(ny-1)*nz); //, Wy, Wz;
  Eigen::SparseMatrix<double> Wz(P.rows(), nx*ny*(nz-1)); //, Wy, Wz;

  fd_interpolate(nx-1, ny, nz, h, corner, P, Wx);
  fd_interpolate(nx, ny-1, nz, h, corner, P, Wy);
  fd_interpolate(nx, ny, nz-1, h, corner, P, Wz);

 // Interpolate Normals
  Eigen::SparseMatrix<double> N_sparse = N.sparseView();
  Eigen::SparseMatrix<double> interp_Nx = Wx.transpose() * N_sparse.col(0);
  Eigen::SparseMatrix<double> interp_Ny = Wy.transpose() * N_sparse.col(1);
  Eigen::SparseMatrix<double> interp_Nz = Wz.transpose() * N_sparse.col(2);

  Eigen::SparseMatrix<double> N_grid = igl::cat(1, interp_Nx, interp_Ny);   
  N_grid = igl::cat(1, N_grid, interp_Nz);

  Eigen::SparseMatrix<double> G((nx-1)*ny*nz+ nx*(ny-1)*nz+ nx*ny*(nz-1), nx*ny*nz);
  fd_grad(nx, ny, nz, h, G);

  Eigen::SparseMatrix<double> GTG = G.transpose() * G;
  Eigen::SparseMatrix<double> GTV = G.transpose() * N_grid;

  Eigen::BiCGSTAB<Eigen::SparseMatrix<double> > solver;
  solver.compute(GTG);

  g = solver.solve(GTV);

  Eigen::SparseMatrix<double> W(P.rows(), nx*ny*nz);
  fd_interpolate(nx,ny,nz, h, corner, P, W);
  double sigma = (Eigen::VectorXd::Ones(n)/(double)n).transpose()*(W*g);
  g = g.array() - sigma;
  igl::copyleft::marching_cubes(g, x, nx, ny, nz, V, F);
}
