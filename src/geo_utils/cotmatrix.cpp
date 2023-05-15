#include "iostream"
#include "geo_utils/cotmatrix.h"
#include <Eigen/Core>
#include <Eigen/Sparse>

void cotmatrix(
  const Eigen::MatrixXd & l,
  const Eigen::MatrixXi & F,
  Eigen::SparseMatrix<double> & L){

    for (int i = 0; i < F.rows(); i++){
      int ix = F(i, 0);
      int iy = F(i, 1);
      int iz = F(i, 2);

      std::cout << ix << " " << iy << " " << iz;
      exit(0);
    }
  }

