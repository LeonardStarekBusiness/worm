#include "worm/functions/function.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "worm/functions/function_mapping.h"
#include "worm/functions/functions.h"
#include "worm/keywords.h"
#include "worm/util/string.h"

namespace {
/*
 * Validates if the signature is complete
 *
 * @return: returns the parameters to this function in the original order
 */
std::vector<std::string> validate_then_return_params(
  const std::string& signature
) {
  auto pos {signature.find(keywords::func_decl)};
  if (pos == std::string::npos) {
    throw std::runtime_error(
      std::string("Error defining function: missing function declaration keyword ")
      + "\"" + keywords::func_decl + "\""
    );
  }

  while (
    pos < signature.size() && std::isspace(static_cast<unsigned char>(signature[pos]))
  ) {
    ++pos;
  }

  std::string func_name;
  while (
    pos < signature.size() && !std::isspace(static_cast<unsigned char>(signature[pos]))
    && signature[pos] != '('
  ) {
    func_name.push_back(signature[pos]);
  }

  if (func_name.empty()) {
    throw std::runtime_error("Error defining function: missing function name!");
  }

  while (
    pos < signature.size() && std::isspace(static_cast<unsigned char>(signature[pos]))
  ) {
    ++pos;
  }

  if (signature[pos] != '(') {
    throw std::runtime_error(
      "Error defining function: missing parameter specification!"
    );
  }

  if (signature.find(')') == std::string::npos) {
    throw std::runtime_error("Error defining function: missing closing parenthesis!");
  }

  if (signature[signature.size() - 1] != ')') {
    throw std::runtime_error(
      "Error defining function: undefined construct after closing parenthesis!"
    );
  }

  assert(signature[pos] == '(');
  assert(signature[signature.size() - 1] == ')');

  return util::string::parse_params(
    std::string_view {
      signature.begin() + static_cast<std::string::difference_type>(pos),
      signature.end()
    }
  );
}
} // namespace

namespace functions {
/*
 * @precondition: Before trying to call the constructor, the calling site's parser
 * should verify that the lines that it passes are the declaration of exactly one
 * function and should only pass the relevant lines
 */
Function::Function(const std::vector<std::string>& lines) {
  if (lines.empty()) {
    throw std::runtime_error("Error defining function: empty function declaration!");
  }

  std::vector<std::string> lines_cp {lines};
  std::for_each(lines_cp.begin(), lines_cp.end(), util::string::trim);

  std::vector params {validate_then_return_params(lines_cp[0])};
  for (std::size_t i {1}; i < lines_cp.size(); ++i) {
    auto [func, mapping] {parse_func_mapping(lines[i], params)};
    sub_funcs.push_back(func);
    param_mapping.push_back(mapping);
  }

  assert(sub_funcs.size() == param_mapping.size() && !base_func);
}

Function::Function(void (*base_func)(const std::vector<std::string>&))
    : base_func {base_func} {
  assert(sub_funcs.empty() && param_mapping.empty());
}

void Function::operator()(const std::vector<std::string>& params) const {
  if (base_func) {
    base_func(params);
    return;
  }

  std::size_t max_param_idx {0};
  for (const auto& vec : param_mapping) {
    const auto& it {std::max_element(vec.begin(), vec.end())};
    if (it != vec.end()) {
      max_param_idx = std::max(*it, max_param_idx);
    }
  }

  if (max_param_idx >= params.size()) {
    throw std::runtime_error(
      "Function requires " + std::to_string(max_param_idx + 1) + " parameters,"
      " but only " + std::to_string(params.size()) + " were provided!"
    );
  }

  std::vector<std::string> cur_params;
  for (std::size_t i {0}; i < sub_funcs.size(); ++i) {
    for (const auto& idx : param_mapping[i]) {
      cur_params.push_back(params[idx]);
    }

    (*sub_funcs[i])(cur_params);
    cur_params.clear();
  }
}

bool Function::operator==(const Function& other) const {
  return this->sub_funcs == other.sub_funcs
    && this->param_mapping == other.param_mapping;
}

bool Function::operator!=(const Function& other) const {
  return !(*this == other);
}
} // namespace functions
