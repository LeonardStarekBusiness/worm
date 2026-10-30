#ifndef WORM_FUNCTIONS_H
#define WORM_FUNCTIONS_H

#include <cstddef>
#include <string>

namespace functions {
class Function;

/* 
 * Interpret a function declaration in Worm and translates it to C++
 * 
 * @throws std::runtime_error if any interpretation error occurs 
 * or if the function already exists with the same signature
 *
 * @return the `Function` that gets declared by this function
 */
Function decl_func(const std::string& func_decl_str);
} // namespace functions

#endif // WORM_FUNCTIONS_H
