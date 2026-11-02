#ifndef WORM_INTERPRETER_H
#define WORM_INTERPRETER_H

#include <cstddef>
#include <cstdio>
#include <deque>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace worm {
class Interpreter {
public:
  Interpreter(std::vector<std::string>);

  void run();

private:
  /*
   * Command-line arguments passed to the interpreter
   */
  std::vector<std::string> args;
  /*
   * Map that stores the local variables
   */
  std::unordered_map<std::string, std::string> vars;
  /*
   * When entering a loop, this deque shall hold all lines of the loop so
   * that the interpreter can "recall" past lines even after moving on
   *
   * The .second parameter of the `std::pair` stores a reference to what 
   * loop point it should jump to on `continue` or when looping naturally
   */
  std::deque<std::pair<std::string, std::size_t>> loop_buf;
  /*
   * Input file (.worm)
   */
  std::ifstream file;
};
} // namespace worm

#endif // WORM_INTERPRETER_H
