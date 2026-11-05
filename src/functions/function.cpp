#include "worm/functions/function.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <exception>
#include <format>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "worm/errors/err_info.h"
#include "worm/functions/function_error.h"
#include "worm/functions/functions.h"
#include "worm/parse/keywords.h"
#include "worm/parse/parse.h"
#include "worm/parse/parse_error.h"
#include "worm/util/string.h"

namespace worm::functions {
namespace {
/*
 * Validates if the signature is complete
 *
 * @return: returns the parameters to this function in the original order
 */
std::pair<std::string, std::vector<std::string>> validate_then_return_params(
  std::string_view signature
) {
  if (!signature.starts_with(parse::keywords::func_decl)) {
    throw parse::ParseError {errors::ErrInfo {
      .msg {
        std::string("Error defining function: missing function declaration keyword ")
        + "\"" + std::string(parse::keywords::func_decl) + "\""
      },
    }};
  }

  std::size_t idx {parse::keywords::func_decl.size()};
  while (
    idx < signature.size() && std::isspace(static_cast<unsigned char>(signature[idx]))
  ) {
    ++idx;
  }

  std::string func_name;
  while (
    idx < signature.size() && !std::isspace(static_cast<unsigned char>(signature[idx]))
    && signature[idx] != '('
  ) {
    func_name.push_back(signature[idx++]);
  }

  if (func_name.empty()) {
    throw parse::ParseError {errors::ErrInfo {
      .msg {"Error defining function: missing function name!"},
    }};
  }

  while (
    idx < signature.size() && std::isspace(static_cast<unsigned char>(signature[idx]))
  ) {
    ++idx;
  }

  try {
    auto diff {static_cast<std::string::difference_type>(idx)};
    return {
      func_name,
      parse::parse_identifiers(
        std::string_view {signature.begin() + diff, signature.end()}
      )
    };
  } catch (const parse::ParseError& e) {
    throw parse::ParseError {errors::ErrInfo {
      .msg {"in `validate_then_return_params()`:"},
      .og_err {std::make_exception_ptr(e)}
    }};
  }
}
} // namespace

std::string Function::hash_code(const std::string& func_name, std::size_t param_count) {
  return func_name + std::to_string(param_count);
}

Function::Function(const std::vector<std::string>& lines) {
  if (lines.empty()) {
    throw FunctionError {errors::ErrInfo {
      .msg {"Error defining function: empty function declaration!"},
    }};
  }

  std::vector<std::string_view> trimmed;
  std::transform(
    lines.begin(),
    lines.end(),
    std::back_inserter(trimmed),
    util::string::trimmed_view
  );

  auto [func_name, params] {validate_then_return_params(trimmed[0])};
  std::size_t line_offset {1};

  for (std::size_t i {1}; i < trimmed.size(); ++i, ++line_offset) {
    try {
      auto [func, mapping] {parse_func_mapping(lines[i], params)};
      sub_funcs.push_back(func);
      param_mapping.push_back(mapping);
    } catch (const FunctionError& e) {
      throw FunctionError {errors::ErrInfo {
        .msg {"in `Function` ctor: "},
        .line_offset = line_offset,
        .og_err {std::make_exception_ptr(e)}
      }};
    }

    assert(sub_funcs.size() == param_mapping.size() && !base_func);
    name = func_name;
    param_count = params.size();
  }
}

Function::Function(
  const std::string& name,
  std::size_t param_count,
  void (*base_func)(const std::vector<std::string>&)
)
    : name {name}
    , param_count {param_count}
    , base_func {base_func} {
  assert(sub_funcs.empty() && param_mapping.empty());
}

void Function::operator()(const std::vector<std::string>& params) const {
  if (param_count < params.size()) {
    throw FunctionError {errors::ErrInfo {
      .msg {std::format(
        "Error executing function `{}`: requires {} parameters, "
        "but only {} were provided!",
        name,
        std::to_string(param_count),
        std::to_string(params.size())
      )},
    }};
  }

  if (base_func) {
    base_func(params);
    return;
  }

  std::vector<std::string> cur_params;
  for (std::size_t i {0}; i < sub_funcs.size(); ++i) {
    for (const auto& [constant, map_idx] : param_mapping[i]) {
      cur_params.push_back(constant.empty() ? params[map_idx] : constant);
    }

    (*sub_funcs[i])(cur_params);
    cur_params.clear();
  }
}

std::string Function::hash_code() const {
  return Function::hash_code(name, param_count);
}

bool Function::operator==(const Function& other) const {
  return this->sub_funcs == other.sub_funcs
    && this->param_mapping == other.param_mapping && this->base_func == other.base_func;
}

bool Function::operator!=(const Function& other) const {
  return !(*this == other);
}
} // namespace worm::functions
