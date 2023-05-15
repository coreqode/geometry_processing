#include "geo_utils/fd_interpolate.h"
#include "iostream"

void fd_interpolate(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const Eigen::RowVector3d & corner,
  const Eigen::MatrixXd & P,
  Eigen::SparseMatrix<double> & W)
{
  for (int i = 0; i < P.rows(); i++){
      float ind_x = (P(i, 0) -  corner(0) ) / h;
      float ind_y = (P(i, 1) -  corner(1) ) / h;
      float ind_z = (P(i, 2) -  corner(2) ) / h;

      int x0 = floor(ind_x);
      int y0 = floor(ind_y);
      int z0 = floor(ind_z);

      float x_norm = ind_x - x0;
      float y_norm = ind_y - y0 ;
      float z_norm = ind_z - z0;
      W.coeffRef(i, x0+nx*(y0+z0*ny)) = (1 - x_norm) * (1 - y_norm) * (1 - z_norm); // (0,0,0)
      W.coeffRef(i, (x0+1)+nx*(y0+z0*ny)) = (x_norm) * (1 - y_norm) * (1 - z_norm); // (1,0,0)
      W.coeffRef(i, x0+nx*((y0+1)+z0*ny)) = (1 - x_norm) * (y_norm) * (1 - z_norm); // (0,1,0)
      W.coeffRef(i, x0+nx*(y0+(z0+1)*ny)) = (1 - x_norm) * (1 - y_norm) * (z_norm); // (0,0,1)
      W.coeffRef(i, (x0+1)+nx*(y0+(z0+1)*ny)) = (x_norm) * (1 - y_norm) * (z_norm); // (1, 0, 1)
      W.coeffRef(i, x0+nx*((y0+1)+(z0+1)*ny)) = (1 - x_norm) * (y_norm) * (z_norm); // (0, 1, 1)
      W.coeffRef(i, (x0+1)+nx*((y0+1)+z0*ny)) = (x_norm) * (y_norm) * (1 - z_norm); // (1,1,0)
      W.coeffRef(i, (x0+1)+nx*((y0+1)+(z0+1)*ny)) = (x_norm) * (y_norm) * (z_norm); // (1,1,1)
    }
}
