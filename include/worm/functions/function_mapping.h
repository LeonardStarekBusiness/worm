#ifndef WORM_FUNCTIONS_FUNCTION_MAPPING_H
#define WORM_FUNCTIONS_FUNCTION_MAPPING_H

#include <cstddef>
#include <vector>

#include "worm/functions/function.h"

namespace worm::functions {
struct FunctionMapping {
  static FunctionMapping noop();

  Function* f;
  std::vector<std::pair<std::string, std::size_t>> mapping;
};
} // namespace worm::functions

#endif // WORM_FUNCTIONS_FUNCTION_MAPPING_H
