#include <iostream>
#include <igl/readOBJ.h>
#include <igl/edge_lengths.h>
#include <igl/cotmatrix.h>
#include <igl/massmatrix.h>
#include <igl/write_triangle_mesh.h>
#include "geo_utils/visualize.h"
#include "geo_utils/parser.h"
#include "igl/map_vertices_to_circle.h"
#include "igl/boundary_loop.h"
#include "igl/min_quad_with_fixed.h"
#include "igl/writeOBJ.h"
#include "igl/adjacency_matrix.h"

int main(int argc, char **argv) {
    std::string filename;
    parser(argc, argv, filename);

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::readOBJ(filename, V, F);

    Eigen::SparseMatrix<double> L(V.rows(), V.rows());
    igl::cotmatrix(V, F, L);
    L = L * -1; //By default cotmatrix gives the negative-definite matrix

    // Indicating u,v with U1,V1
    Eigen::MatrixXd U(V.rows(), 2);
    U.setZero();
    Eigen::MatrixXd b;
    b.setZero();
    Eigen::SparseMatrix<double> Aeq;
    Aeq.setZero();

    Eigen::VectorXi bnd;
    Eigen::MatrixXd bnd_uv;
    igl::boundary_loop(F, bnd);
    igl::map_vertices_to_circle(V, bnd, bnd_uv);

    igl::min_quad_with_fixed_data<double> mqwf;
    igl::min_quad_with_fixed_precompute(L, bnd, Aeq, true, mqwf);

    Eigen::VectorXd B(L.rows(), 1);
    B.setZero();
    Eigen::VectorXd Beq;
    Eigen::MatrixXd U1 = U.col(0);
    Eigen::MatrixXd U2 = U.col(1);
    igl::min_quad_with_fixed_solve(mqwf, B, bnd_uv.col(0), Beq, U1);
    igl::min_quad_with_fixed_solve(mqwf, B, bnd_uv.col(1), Beq, U2);

    U = igl::cat(2, U1, U2);
    U = (U.array() - U.minCoeff()) / (U.maxCoeff() - U.minCoeff());

    Eigen::MatrixXd CN;
    Eigen::MatrixXd FN;
    Eigen::MatrixXd FTC;

    igl::writeOBJ("../output/param.obj", V, F, CN, FN, U, F);

    return 0;
}