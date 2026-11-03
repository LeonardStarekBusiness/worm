#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "worm/interpreter.h"

std::vector<std::string> argv_to_vec(int argc, const char** argv) {
  std::vector<std::string> ret;
  for (int i {0}; i < argc; ++i) {
    ret.push_back(argv[i]);
  }

  return ret;
}

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cout << "Error: no .worm input file provided!" << std::endl;
    return 1;
  }

  try {
    worm::Interpreter interpreter {argv_to_vec(argc, argv)};
    interpreter.run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
}
