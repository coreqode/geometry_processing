#include "geo_utils/fd_partial_derivative.h"

void fd_partial_derivative(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const int dir,
  Eigen::SparseMatrix<double> & D)
{
  int m_x = nx - (dir == 0);
  int m_y = ny - (dir == 1);
  int m_z = nz - (dir == 2);

  for (int i = 0; i < m_x; i++){
    for (int j = 0; j < m_y; j++){
      for (int k = 0; k < m_z; k++){
        int ind_0 = i+nx*(j+k*ny);
        int _i = i+(dir==0);
        int _j = j+(dir==1);
        int _k = k+(dir==2);
        int ind_1 = _i+nx*(_j+_k*ny);
        int ind_stg = i+m_x*(j+k*m_y);

        D.coeffRef(ind_stg, ind_0) = -1 / h;
        D.coeffRef(ind_stg, ind_1) = 1 / h;
      }
    }
  }
}
