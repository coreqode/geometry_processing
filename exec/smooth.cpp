#include <iostream>
#include "args/args.hxx"
#include <igl/readOBJ.h>
#include <igl/edge_lengths.h>
#include <igl/cotmatrix.h>
#include <igl/massmatrix.h>
#include <igl/write_triangle_mesh.h>
#include "geo_utils/visualize.h"


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
  Eigen::MatrixXd meshV;
  Eigen::MatrixXi meshF;
  igl::readOBJ(filename, meshV, meshF);

  // visualize(meshV, meshF);
  Eigen::MatrixXd el(meshF.rows(), 3);
  igl::edge_lengths(meshV, meshF, el);

  // cotmatrix(el, meshF, L);

  // Make noisy mesh
  meshV = meshV.array() - meshV.minCoeff();
  meshV = meshV.array() / (meshV.maxCoeff() - meshV.minCoeff());
  Eigen::MatrixXd noisyMeshV(meshV.rows(), meshV.cols());
  Eigen::MatrixXd rand;
  rand = Eigen::MatrixXd::Random(meshV.rows(), meshV.cols());


  // Smooth the noisy mesh
  float NOISE = 0.02;
  int ITER = 5;
  float LAMBDA = 0.0001;

  noisyMeshV = meshV + NOISE * rand;
  Eigen::MatrixXd initV ;
  initV = noisyMeshV;

  Eigen::SparseMatrix<double> L(meshV.rows(), meshV.rows());
  Eigen::SparseMatrix<double> M(meshV.rows(), meshV.rows());

  std::string out_file = "../output/noisy.ply";
  igl::write_triangle_mesh(out_file, initV, meshF);

  for (int i = 0; i < ITER; i ++){
    
    std::cout << "Smoothing for "<< i+1 <<"st iteration!!" <<std::endl;
    L.setZero();
    M.setZero();
    // Calculate the cotanget and massmatrix
    igl::cotmatrix(initV, meshF, L);
    igl::massmatrix(meshV,meshF,igl::MASSMATRIX_TYPE_VORONOI,M);

    Eigen::SparseMatrix<double> LHS, RHS;
    LHS = meshV.sparseView();
    LHS = M * LHS;
    RHS = LAMBDA * L;
    RHS = M - RHS;

    Eigen::SparseLU<Eigen::SparseMatrix<double> > solver;
    solver.compute(RHS);
    Eigen::SparseMatrix<double> I(meshV.rows(),meshV.rows());
    I.setIdentity();
    auto RHS_inv = solver.solve(I);
    Eigen::SparseMatrix<double> newVerts = RHS_inv * LHS;
    initV = Eigen::MatrixXd(newVerts);

    std::string out_file = "../output/smooth_" + std::to_string(i) + ".ply";
    igl::write_triangle_mesh(out_file, initV, meshF);
  }

  // visualize(initV, meshF);
  return 0;
}
