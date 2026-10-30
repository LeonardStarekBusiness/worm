#ifndef WORM_FUNCTIONS_H
#define WORM_FUNCTIONS_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace functions {
class Function;
struct FunctionMapping;

/*
 * Used to keep track of all the Worm-functions declared at runtime or of
 * base-functions like print()
 * New function declarations should be added into this map
 *
 * This map defines the source of truth which functions already exist and
 * should be used to prevent duplicate functions that would create ambiguity
 */
extern std::unordered_map<std::string, Function> funcs;

/*
 * (NOT YET IMPLEMENTED, NOR FOUND THE PLACE WHERE THIS IS NEEDED)
 *
 * Interprets a function declaration in Worm and translates it to C++
 *
 * @throw: throws std::runtime_error if any interpretation error occurs
 * or if the function already exists with the same signature
 *
 * @return: returns the `Function` that gets declared by this function
 */
Function decl_func(const std::string& func_decl_str);
/*
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
 */
FunctionMapping parse_func_mapping(std::string_view, const std::vector<std::string>);
} // namespace functions

#endif // WORM_FUNCTIONS_H
