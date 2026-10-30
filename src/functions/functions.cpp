#include "worm/functions/functions.h"

#include <string>
#include <unordered_map>

#include "worm/functions/function.h"
#include "worm/wstd.h"

namespace functions {
std::unordered_map<std::string, Function> funcs {
  {"print", Function {&wstd::print}},
  {"println", Function {&wstd::println}}
};
} // namespace functions
