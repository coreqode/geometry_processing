#ifndef VISUALIZE_MESH_H
#define VISUALIZE_MESH_H
#include <Eigen/Core>
#include <Eigen/Sparse>

void visualize(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F);

#endif
