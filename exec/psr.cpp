#include <iostream>
#include "args/args.hxx"
#include <igl/readOBJ.h>
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/point_cloud.h"
#include "psr/poisson_surface_reconstruction.h"

int main(int argc, char **argv) {
  // Configure the argument parser
  args::ArgumentParser parser("");
  args::Positional<std::string> inFile(parser, "mesh", "input mesh");

  try {
    parser.ParseCLI(argc, argv);
  } catch (args::Help) {
    std::cout << parser;
    return 0;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;

    std::cerr << parser;
    return 1;
  }

  std::string filename = args::get(inFile);
  std::cout << "loading: " << filename << std::endl;

  // Read the mesh
  // igl::readOBJ(filename, meshV, meshF);
   Eigen::MatrixXd P,N;
  {
    Eigen::MatrixXd D;
    std::vector<std::vector<double> > vD;
    std::string line;
    std::fstream in;
    in.open(argc>1?argv[1]:"../data/elephant.pwn");
    while(in)
    {
      std::getline(in, line);
      std::vector<double> row;
      std::stringstream stream_line(line);
      double value;
      while(stream_line >> value) row.push_back(value);
      if(!row.empty()) vD.push_back(row);
    }
    if(!igl::list_to_matrix(vD,D)) return EXIT_FAILURE;
    assert(D.cols() == 6 && "pwn file should have 6 columns");
    P = D.leftCols(3);
    N = D.rightCols(3);
  }

  Eigen::MatrixXd meshV;
  Eigen::MatrixXi meshF;

  poisson_surface_reconstruction(P, N, meshV, meshF);

  // Register the mesh with Polyscope
  polyscope::options::autocenterStructures = true;
  polyscope::view::windowWidth = 1024;
  polyscope::view::windowHeight = 1024;
  polyscope::init();
  polyscope::registerSurfaceMesh("mesh", meshV, meshF);
  polyscope::registerPointCloud("input point cloud", P);
  polyscope::getPointCloud("input point cloud")
      ->addVectorQuantity("normals", N);
  polyscope::show();

  return 0;
}
