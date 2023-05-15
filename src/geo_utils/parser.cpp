#include "geo_utils/parser.h"

int parser(int argc, char **argv, std::string &filename)
{
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
  filename = args::get(inFile);
}