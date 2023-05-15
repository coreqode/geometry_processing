#include <iostream>
#include <Eigen/Core>
#include <Eigen/Sparse>

void print_shape(Eigen::MatrixXd matrix){
    std::cout << "Rows: " << matrix.rows() << " " << "Cols: " << matrix.cols() << std::endl;
}