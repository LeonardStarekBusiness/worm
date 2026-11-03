#include "worm/functions/function.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "worm/functions/function_mapping.h"
#include "worm/functions/functions.h"
#include "worm/parse/keywords.h"
#include "worm/parse/parse.h"
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
    throw std::invalid_argument(
      std::string("Error defining function: missing function declaration keyword ")
      + "\"" + std::string(parse::keywords::func_decl) + "\""
    );
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
    throw std::invalid_argument("Error defining function: missing function name!");
  }

  while (
    idx < signature.size() && std::isspace(static_cast<unsigned char>(signature[idx]))
  ) {
    ++idx;
  }

  try {
    return {
      func_name,
      parse::parse_identifiers(
        std::string_view {
          signature.begin() + static_cast<std::string::difference_type>(idx),
          signature.end()
        }
      )
    };
  } catch (std::invalid_argument& e) {
    throw std::invalid_argument(
      std::string("in validate_then_return_params(" + std::string(signature) + "):\n")
      + e.what()
    );
  }
}
} // namespace

/*
 * @precondition: Before trying to call the constructor, the calling site's parser
 * should verify that the lines that it passes are the declaration of exactly one
 * function and should only pass the relevant lines
 */
Function::Function(const std::vector<std::string>& lines) {
  if (lines.empty()) {
    throw std::invalid_argument("Error defining function: empty function declaration!");
  }

  std::vector<std::string_view> trimmed;
  std::transform(
    lines.begin(),
    lines.end(),
    std::back_inserter(trimmed),
    util::string::trimmed_view
  );

  auto [func_name, params] {validate_then_return_params(trimmed[0])};
  for (std::size_t i {1}; i < trimmed.size(); ++i) {
    auto [func, mapping] {parse_func_mapping(lines[i], params)};
    sub_funcs.push_back(func);
    param_mapping.push_back(mapping);
  }

  assert(sub_funcs.size() == param_mapping.size() && !base_func);
  name = func_name;
  param_count = params.size();
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
    throw std::invalid_argument(
      "Function requires " + std::to_string(param_count) + " parameters,"
      " but only " + std::to_string(params.size()) + " were provided!"
    );
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
  return name + std::to_string(param_count);
}

bool Function::operator==(const Function& other) const {
  return this->sub_funcs == other.sub_funcs
    && this->param_mapping == other.param_mapping && this->base_func == other.base_func;
}

bool Function::operator!=(const Function& other) const {
  return !(*this == other);
}
} // namespace worm::functions
