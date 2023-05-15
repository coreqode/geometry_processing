#include <iostream>
#include "args/args.hxx"
#include <igl/read_triangle_mesh.h>
#include "geo_utils/visualize.h"
#include <glob.h>
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include <string.h>
#include <stdio.h>


int main(int argc, char **argv) {
  // Configure the argument parser
  args::ArgumentParser parser("");
  args::Positional<std::string> inFile(parser, "folder_path", "folder_path");

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

  std::string folder_path = args::get(inFile);
  folder_path = folder_path + "*.ply";

  std::vector<std::string> filenames;
  glob_t glob_result;
  int return_value = glob(const_cast<char*>(folder_path.c_str()), 0, nullptr, &glob_result);
  if (return_value == 0) {
    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        filenames.push_back(glob_result.gl_pathv[i]);
        }
  }
  globfree(&glob_result);

  polyscope::options::autocenterStructures = true;
  polyscope::view::windowWidth = 1024;
  polyscope::view::windowHeight = 1024;
  polyscope::init();
  for (auto const filepath : filenames){
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    std::cout << filepath << std::endl;
    igl::read_triangle_mesh(filepath, V, F);

    char *name = std::strtok(const_cast<char*>(filepath.c_str()), "/");
    while (name != NULL)
    {
        name = std::strtok(NULL, "/");

        if (name != NULL){
            polyscope::registerSurfaceMesh(name, V, F);
        }
    }
  }
  polyscope::show();
}