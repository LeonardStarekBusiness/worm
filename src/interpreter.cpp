#include "worm/interpreter.h"

#include <cctype>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "worm/functions/functions.h"
#include "worm/parse/construct.h"
#include "worm/parse/parse.h"
#include "worm/util/string.h"

namespace worm {
Interpreter::Interpreter(std::vector<std::string> args) : args {args} {
  if (args.empty() || !args[0].ends_with(".worm")) {
    throw std::invalid_argument(
      "Error initalizing interpreter: No .worm file to execute!"
    );
  }

  /*
   * This is the place to add command-line argument validation or implement logic
   * for flags that can be passed to the interpreter
   */

  file = std::ifstream {args[0]};
  if (!file) {
    throw std::runtime_error(
      "Error initializing interpreter: Couldn't open file '" + args[0] + "'!"
    );
  }
}

void Interpreter::run() {
  const std::size_t indent_width {4};
  std::size_t indent_lvl {0};
  std::string line;
  bool reread_line {false};
  std::ifstream::pos_type last_pos {};

  while (std::getline(file, line)) {
    if (reread_line) {
      file.clear();
      file.seekg(last_pos);
      reread_line = false;
    }

    if (line.empty()) {
      continue;
    }

    if (
      auto cur_indent {util::string::indent_lvl(line)}; !(cur_indent % indent_width)
    ) {
      throw std::invalid_argument(
        "Error reading .worm source code: sloppy indentation!"
      );
    }

    switch (parse::determine_construct(line)) {
      case Construct::FuncDecl: {
        std::vector func_def_lines {line};
        std::size_t base_indent_lvl {indent_lvl};

        while (
          std::getline(file, line) && util::string::indent_lvl(line) >= base_indent_lvl
        ) {
          if (line.empty()) {
            continue;
          }

          auto cur_indent {util::string::indent_lvl(line)};
          if (!(cur_indent % indent_width)) {
            throw std::invalid_argument(
              "Error reading .worm source code: sloppy indentation!"
            );
          }

          if (cur_indent >= base_indent_lvl) {
            last_pos = file.tellg();
            reread_line = true;
            break;
          }

          func_def_lines.push_back(line);
        }

        functions::def_func(func_def_lines);
        break;
      }
      case Construct::Statement: {

        break;
      }
      default: throw std::logic_error("not yet implemented!");
    }
  }
}
} // namespace worm
