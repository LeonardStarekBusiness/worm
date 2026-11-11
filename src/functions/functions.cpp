#include "worm/functions/functions.h"

#include <algorithm>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "worm/errors/err_info.h"
#include "worm/errors/interpreter_error.h"
#include "worm/functions/function.h"
#include "worm/functions/function_error.h"
#include "worm/functions/function_mapping.h"
#include "worm/parse/parse.h"
#include "worm/parse/parse_error.h"
#include "worm/util/string.h"
#include "worm/wstd.h"

namespace worm::functions {
namespace {
Function noop_func {"noop", 0, [](const auto&) {}};
Function print_n {"print", 1, &wstd::print_n, true};
Function println_n {"println", 1, &wstd::println_n, true};
Function sleep_ms_1 {"sleep_ms", 1, &wstd::sleep_ms_1};
} // namespace

std::unordered_map<std::string, Function> funcs {
  {print_n.hash_code(), print_n},
  {println_n.hash_code(), println_n},
  {sleep_ms_1.hash_code(), sleep_ms_1},
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
    throw FunctionError {errors::ErrInfo {
      .msg {std::format("Error executing function: `{}` is not a function call!", sv)}
    }};
  }

  std::string func_name {sv.begin(), func_name_end_it};
  util::string::trim(func_name);

  std::vector<std::string> params;
  try {
    params = parse::parse_params({func_name_end_it, sv.end()});
  } catch (const parse::ParseError& e) {
    throw parse::ParseError {errors::ErrInfo {
      .msg {
        std::format("in `worm::functions::substitute_execute({})`:\n{}", sv, e.what())
      },
      .line_offset = e.err_info.line_offset,
      .og_err {std::make_exception_ptr(e)}
    }};
  }

  std::string func_hash {Function::hash_code(func_name, params.size())};
  auto func_it {funcs.find(func_hash)};

  if (func_it == funcs.end()) {
    std::string alt_hash {Function::hash_code(func_name, 0, true)};
    func_it = funcs.find(alt_hash);

    if (func_it == funcs.end()) {
      throw FunctionError {errors::ErrInfo {.msg {std::format(
        "Error executing function call: undefined reference to "
        "\x1b[3;1m{}\x1b[0m!",
        func_hash
      )}}};
    }
  }

  for (auto& param : params) {
    if (!std::all_of(param.begin(), param.end(), parse::is_identifier_char)) {
      util::string::unescape_str_delims(param);
      continue;
    }

    auto it {map.find(param)};
    if (it == map.end()) {
      throw parse::ParseError {errors::ErrInfo {.msg {std::format(
        "Error parsing statement: use of undeclared identifier '{}'!",
        param
      )}}};
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
      throw FunctionError {errors::ErrInfo {.msg {std::format(
        "Error declaring function with hash '{}': function already exists!",
        hash
      )}}};
    }

    return it->second;
  } catch (const errors::InterpreterError& e) {
    throw errors::InterpreterError {errors::ErrInfo {
      .msg {"in `worm::functions::def_func()`:"},
      .line_offset = e.err_info.line_offset,
      .og_err {std::make_exception_ptr(e)}
    }};
  }
}

/*
 * Currently only parses a single-function-statement and doesn't handle nested
 * function calls or expressions for that matter
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
  } catch (const parse::ParseError& e) {
    throw parse::ParseError {
      errors::ErrInfo {.msg {"in `worm::functions::parse_func_mapping()`:"}}
    };
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
      throw parse::ParseError {errors::ErrInfo {.msg {std::format(
        "Error parsing statement: use of undeclared identifier '{}'!",
        param
      )}}};
    }

    if (is_identifier) {
      auto param_idx {static_cast<std::size_t>(std::distance(params.begin(), it))};
      mapping.emplace_back("", param_idx);
    } else {
      mapping.emplace_back(param, 0);
    }
  }

  std::string func_hash {Function::hash_code(func_name, func_params.size())};
  auto func_it {funcs.find(func_hash)};

  if (func_it == funcs.end()) {
    throw FunctionError {errors::ErrInfo {.msg {std::format(
      "Error mapping statement: use of undeclared function with hash '{}'!",
      func_hash
    )}}};
  }

  return {&func_it->second, mapping};
}
} // namespace worm::functions
