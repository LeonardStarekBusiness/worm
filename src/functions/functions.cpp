#include "worm/functions/functions.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "worm/functions/function.h"
#include "worm/functions/function_mapping.h"
#include "worm/parse/parse.h"
#include "worm/wstd.h"

namespace worm::functions {
std::unordered_map<std::string, Function> funcs {
  {"print0", Function {"print", &wstd::print}},
  {"println0", Function {"println", &wstd::println}}
};

Function def_func(const std::vector<std::string>& lines) {
  try {
    Function f {lines};
    auto hash {f.hash_code()};

    auto [it, inserted] = funcs.try_emplace(hash, std::move(f));
    if (!inserted) {
      throw std::invalid_argument(
        "Error declaring function with hash '" + hash + "': function already exists!"
      );
    }

    return it->second;
  } catch (std::invalid_argument& e) {
    throw std::invalid_argument(
      std::string("In `worm::functions::def_func()`:\n") + e.what()
    );
  }
}

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
  std::vector func_params {parse::parse_params({func_name_end_it, line.end()})};
  auto func_it {funcs.find({func_name, func_params.size()})};

  if (func_it == funcs.end()) {
    throw std::runtime_error(
      "Error parsing statement: use of undeclared function '" + func_name + "'!"
    );
  }

  std::vector<std::size_t> mapping;
  for (const auto& param : func_params) {
    auto it {std::find(params.begin(), params.end(), param)};
    if (it != params.end()) {
      mapping.push_back(static_cast<std::size_t>(std::distance(params.begin(), it)));
      continue;
    }

    if (parse::identifier_len(param) == param.size()) {
      throw std::runtime_error(
        "Error parsing statement: use of undeclared identifier '" + param + "'!"
      );
    }

    throw std::logic_error(
      "TODO: Implement behavior when a function takes a constant as argument"
      // mapping.push_back(-1);
    );
  }

  return {&func_it->second, mapping};
}
} // namespace worm::functions
