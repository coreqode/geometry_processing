#include "geo_utils/fd_grad.h"
#include "iostream"
#include "igl/cat.h" 

void fd_grad(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  Eigen::SparseMatrix<double> & G)
{
  int size_x = (nx-1)*ny*nz;
  int size_y = nx*(ny-1)*nz;
  int size_z = nx*ny*(nz-1);
  int total_size = size_x + size_y + size_z; 

  Eigen::SparseMatrix<double> D_x(size_x, nx*ny*nz);
  Eigen::SparseMatrix<double> D_y(size_y, nx*ny*nz);
  Eigen::SparseMatrix<double> D_z(size_z, nx*ny*nz);

  fd_partial_derivative(nx, ny, nz, h, 0, D_x);
  fd_partial_derivative(nx, ny, nz, h, 1, D_y);
  fd_partial_derivative(nx, ny, nz, h, 2, D_z);

  std::cout << "Partial Derivative Matrices are created!!!" << std::endl;

  G = igl::cat(1, D_x, D_y);
  G = igl::cat(1, G, D_z);

  std::cout << "Gradient Matrices created!!!" << std::endl;
}
