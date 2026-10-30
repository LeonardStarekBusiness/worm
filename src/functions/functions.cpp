#include "worm/functions/functions.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "worm/functions/function.h"
#include "worm/functions/function_mapping.h"
#include "worm/util/string.h"
#include "worm/wstd.h"

namespace functions {
std::unordered_map<std::string, Function> funcs {
  {"print", Function {&wstd::print}},
  {"println", Function {&wstd::println}}
};

/*
 * Currently only parses a single-function-statement and doesn't handle nested function
 * calls or expressions for that matter
 *
 * Not handling nested function calls is sensible at this state as functions with 
 * return types aren't implemented yet to begin with
 */
FunctionMapping parse_func_mapping(
  std::string_view line,
  const std::vector<std::string>& params
) {
  auto func_name_end_it {std::find(line.begin(), line.end(), '(')};
  bool valid_func_name {std::all_of(line.begin(), func_name_end_it, [](auto ch) {
    auto uch {static_cast<unsigned char>(ch)};
    return std::isalpha(uch);
  })};

  if (!valid_func_name) {
    throw std::runtime_error(
      "Error parsing statement: function name contains invalid chars!"
    );
  }

  std::string func_name {line.begin(), func_name_end_it};
  auto func_it {funcs.find(func_name)};

  if (func_it == funcs.end()) {
    throw std::runtime_error(
      "Error parsing statement: use of undeclared function '" + func_name + "'!"
    );
  }

  std::vector func_params {util::string::parse_params({func_name_end_it, line.end()})};
  std::vector<std::size_t> mapping;

  for (const auto& param : func_params) {
    auto it {std::find(params.begin(), params.end(), param)};
    if (it == params.end()) {
      throw std::runtime_error(
        "Error parsing statement: use of undeclared identifier '" + param + "'!"
      );
    }

    mapping.push_back(static_cast<std::size_t>(std::distance(params.begin(), it)));
  }

  return {&func_it->second, mapping};
}
} // namespace functions
