#ifndef WORM_FUNCTIONS_H
#define WORM_FUNCTIONS_H

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "worm/functions/function.h"
#include "worm/functions/function_mapping.h"

namespace worm::functions {
/*
 * Used to handle NO-OPs (e.g. when an empty line occurs, this might help, depending on
 * parser implementation)
 */
extern Function noop_func;

/*
 * Used to keep track of all the Worm-functions declared at runtime or of
 * base-functions like print()
 * New function declarations should be added into this map
 *
 * This map defines the source of truth which functions already exist and
 * should be used to prevent duplicate functions that would create ambiguity
 *
 * Key: `Function::hash_code`
 * Value: (callable) `Function`
 */
extern std::unordered_map<std::string, Function> funcs;

/*
 * Substitutes all of the map's keys with their values and leaves other variables alone,
 * then exectues the resulting statement
 */
void substitute_execute(
  std::string_view,
  std::unordered_map<std::string, std::string>&
);
/*
 * Interprets a function declaration in Worm and translates it to C++
 *
 * @throw: throws if any interpretation error occurs
 * or if the function already exists with the same signature
 *
 * @return: returns the `Function` that gets declared by this function
 */
Function def_func(const std::vector<std::string>&);
/*
 * Creates a `FunctionMapping` out of a .worm input line and a parameter list
 *
 * Example:
 * .worm: "print(a, x, z)
 * input parameters: [a, b, c, x, y, z]
 * output: FunctionMapping {.f: &wstd::print, .mapping: [0, 3, 5]}
 *
 * @precondition: this function relies on the fact that the input line has
 * been trimmed before being passed
 *
 * @param: takes a string input line (.worm) and a string parameter list
 *
 * @throw: throws if no function is declared that matches the signature of the
 * parsed function
 *
 * @return: returns a `FunctionMapping` which holds the referred-to function
 * in the input line and the parameter mapping that that function takes
 *
 * Edge cases:
 * If an empty string view is passed, returns the mapping of `noop_func`
 */
FunctionMapping parse_func_mapping(std::string_view, const std::vector<std::string>&);
} // namespace worm::functions

#endif // WORM_FUNCTIONS_H
