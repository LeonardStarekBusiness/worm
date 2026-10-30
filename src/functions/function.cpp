#include "worm/functions/function.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace functions {
/*
 * TODO: Implement constructors
 * verify that `sub_funcs` and `param_mapping` have the same size
 * And that only base_func or the other two fields are set to anything
 */

Function::Function(const std::vector<std::string>& lines) {
  // TODO: Implement
}

Function::Function(void (*base_func)(const std::vector<std::string>&))
    : base_func {base_func} {}

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
