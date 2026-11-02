#include "worm/functions/functions.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "worm/functions/function.h"
#include "worm/functions/function_mapping.h"
#include "worm/parse/parse.h"
#include "worm/util/string.h"
#include "worm/wstd.h"

namespace worm::functions {
namespace {
void noop(const std::vector<std::string>&) {}
} // namespace

Function noop_func {"__noop", &noop};

std::unordered_map<std::string, Function> funcs {
  {noop_func.hash_code(), noop_func},
  {"print0", Function {"print", &wstd::print}},
  {"println0", Function {"println", &wstd::println}}
};

void substitute_execute(
  std::string_view sv,
  std::unordered_map<std::string, std::string>& map
) {
  if (sv.empty()) {
    return;
  }

  auto func_name_end_it {std::find(sv.begin(), sv.end(), '(')};
  if (func_name_end_it == sv.end()) {
    throw std::runtime_error("Error executing function call: not a function call!");
  }

  std::string func_name {sv.begin(), func_name_end_it};
  util::string::trim(func_name);

  std::vector params {parse::parse_params({sv.begin(), func_name_end_it})};
  func_name += std::to_string(params.size());

  auto func_it {funcs.find(func_name)};
  if (func_it == funcs.end()) {
    throw std::runtime_error(
      "Error executing function call: undefined function '" + func_name + "'!"
    );
  }

  for (auto& param : params) {
    if (!std::all_of(param.begin(), param.end(), parse::is_identifier_char)) {
      continue;
    }

    auto it {map.find(param)};
    if (it == map.end()) {
      throw std::runtime_error(
        "Error parsing statement: use of undeclared identifier '" + param + "'!"
      );
    }

    param = it->second;
  }

  (func_it->second)(params);
}

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
  if (line.empty()) {
    return FunctionMapping {.f = &noop_func, .mapping = {}};
  }

  auto func_name_end_it {std::find(line.begin(), line.end(), '(')};
  std::string func_name {line.begin(), func_name_end_it};
  util::string::trim(func_name);

  std::vector<std::string> func_params;
  try {
    func_params = parse::parse_params({func_name_end_it, line.end()});
  } catch (std::invalid_argument& e) {
    throw std::invalid_argument(
      std::format("in `worm::functions::parse_func_mapping()`:\n{}", e.what())
    );
  }

  std::vector<std::pair<std::string, std::size_t>> mapping;
  for (const auto& param : func_params) {
    auto it {std::find(params.begin(), params.end(), param)};
    bool is_identifier {
      std::all_of(param.begin(), param.end(), parse::is_identifier_char)
    };

    if (
      is_identifier && std::find(params.begin(), params.end(), param) == params.end()
    ) {
      throw std::runtime_error(
        "Error parsing statement: use of undeclared identifier '" + param + "'!"
      );
    }

    if (is_identifier) {
      auto param_idx {static_cast<std::size_t>(std::distance(params.begin(), it))};
      mapping.emplace_back("", param_idx);
    } else {
      mapping.emplace_back(param, 0);
    }
  }

  auto func_it {funcs.find(func_name)};
  if (func_it == funcs.end()) {
    throw std::runtime_error(
      "Error parsing statement: use of undeclared function '" + func_name + "'!"
    );
  }

  return {&func_it->second, mapping};
}
} // namespace worm::functions
