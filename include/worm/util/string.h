#ifndef WORM_UTIL_STRING_H
#define WORM_UTIL_STRING_H

#include <string>
#include <string_view>
#include <vector>

namespace util::string {
/*
 * Removes any leading or trailing whitespaces
 */
void trim(std::string&);
/*
 * Parses parameters in the form "(x, y, z)" (spaces after the commas are ignored)
 *
 * @throw: throws if there is no outer-level pair of parentheses
 * or if any non-alphabetical chars are used in the identifier names 
 * (no digits allowed either)
 *
 * @return: returns the identifiers inside the parentheses, the result should contain
 * all comma-separated values
 */
std::vector<std::string> parse_params(std::string_view);
}

#endif // WORM_UTIL_STRING_H
