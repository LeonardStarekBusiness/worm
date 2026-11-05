#ifndef WORM_WSTD_H
#define WORM_WSTD_H

#include <string>
#include <vector>

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
namespace worm::wstd {
void print_n(const std::vector<std::string>&);
void println_n(const std::vector<std::string>&);
void sleep_1(const std::vector<std::string>&);
} // namespace worm::wstd

#endif // WORM_WSTD_H
