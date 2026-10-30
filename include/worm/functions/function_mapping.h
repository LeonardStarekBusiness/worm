#ifndef WORM_FUNCTIONS_FUNCTION_MAPPING_H
#define WORM_FUNCTIONS_FUNCTION_MAPPING_H

#include <cstddef>
#include <vector>

#include "worm/functions/function.h"

namespace functions {
struct FunctionMapping {
  Function* f;
  std::vector<std::size_t> mapping;
};
} // namespace functions

#endif // WORM_FUNCTIONS_FUNCTION_MAPPING_H
