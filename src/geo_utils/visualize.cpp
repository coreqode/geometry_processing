#include "geo_utils/visualize.h"
#include "iostream"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"

void visualize(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F){

  polyscope::options::autocenterStructures = true;
  polyscope::view::windowWidth = 1024;
  polyscope::view::windowHeight = 1024;
  polyscope::init();
  polyscope::registerSurfaceMesh("mesh", V, F);
  polyscope::show();
}
