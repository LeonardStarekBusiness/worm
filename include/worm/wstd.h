#ifndef WORM_WSTD_H
#define WORM_WSTD_H

#include <string>

/*
 * wstd stands for Worm-STandarD-library
 * 
 * Defines the direct interface between built-in worm commands and functions callable 
 * from C++ 
 *
 * Example: 
 * .worm: print(x) 
 * --delegate to--> .cpp: wstd::print(x) 
 * --converts it into--> std::cout << x
 */
namespace wstd {
void decl_func(const std::string& func_decl_str);
void print(const std::string& s);
} // namespace wstd

#endif // WORM_WSTD_H
