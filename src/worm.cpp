#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "worm/errors/interpreter_error.h"
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
  } catch (worm::errors::InterpreterError& e) {
    std::cout << "\x1b[31mIntepreter-error occurred\x1b[0m:\n" << e.what() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cout << "\x1b[41mInternal error occurred\x1b[0m:\n"
              << e.what() << std::endl;
    return 1;
  }

  return 0;
}
