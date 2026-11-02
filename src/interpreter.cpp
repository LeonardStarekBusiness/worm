#include "worm/interpreter.h"

#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "worm/functions/functions.h"
#include "worm/parse/construct.h"
#include "worm/parse/parse.h"
#include "worm/util/string.h"

namespace worm {
namespace {
std::ifstream& nextline_check_indent(
  std::ifstream& file,
  std::string& buf,
  std::size_t& indent_lvl,
  const std::size_t indent_width
) {
  std::getline(file, buf);
  indent_lvl = util::string::indent_lvl(buf);

  if (indent_lvl % indent_width) {
    throw std::invalid_argument("Error reading .worm source code: sloppy indentation!");
  }

  return file;
}
} // namespace

Interpreter::Interpreter(const std::vector<std::string>& args) : args {args} {
  if (args.size() < 2 || !args[1].ends_with(".worm")) {
    throw std::invalid_argument(
      "Error initalizing interpreter: No .worm file to execute!"
    );
  }

  /*
   * This is the place to add command-line argument validation or implement logic
   * for flags that can be passed to the interpreter
   */

  file = std::ifstream {args[1]};
  if (!file) {
    throw std::runtime_error(
      "Error initializing interpreter: Couldn't open file '" + args[1] + "'!"
    );
  }
}

void Interpreter::run() {
  const std::size_t indent_width {2};
  std::size_t indent_lvl {0};
  [[maybe_unused]]
  std::size_t line_pos {0};
  std::string line;
  bool reread_line {false};
  std::ifstream::pos_type last_pos {};

  try {
    while (nextline_check_indent(file, line, indent_lvl, indent_width)) {
      if (reread_line) {
        file.clear();
        file.seekg(last_pos);
        nextline_check_indent(file, line, indent_lvl, indent_width);
        reread_line = false;
      }

      ++line_pos;
      if (line.empty()) {
        continue;
      }

      switch (parse::determine_construct(line)) {
        case Construct::FuncDecl: {
          std::vector func_def_lines {line};
          std::size_t base_indent_lvl {indent_lvl};

          while (nextline_check_indent(file, line, indent_lvl, indent_width)) {
            ++line_pos;
            if (line.empty()) {
              last_pos = file.tellg();
              continue;
            }

            auto cur_indent {util::string::indent_lvl(line)};
            if (cur_indent <= base_indent_lvl) {
              reread_line = true;
              --line_pos;
              break;
            }

            func_def_lines.push_back(line);
            last_pos = file.tellg();
          }

          functions::def_func(func_def_lines);
          break;
        }
        case Construct::Statement: {
          functions::substitute_execute(line, vars);
          break;
        }
        default: throw std::logic_error("not yet implemented!");
      }
    }
  } catch (std::exception& e) {
    throw std::runtime_error("At line " + std::to_string(line_pos) + ":\n" + e.what());
  }
}
} // namespace worm
