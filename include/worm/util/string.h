#ifndef WORM_UTIL_STRING_H
#define WORM_UTIL_STRING_H

#include <string>
#include <string_view>
#include <vector>

namespace worm::util::string {
/*
 * Removes any leading or trailing whitespaces
 */
void trim(std::string&);
/*
 * NOTE: NOT YET IMPLEMENTED
 *
 * Parses parameters like `worm::util::string::parse_identifiers()`, just that
 * it also allows for string literals (expressions wrapped in ") and numeric
 * constants (like 42, 69)
 */
std::vector<std::string> parse_params(std::string_view);
/*
 * Parses parameters in the form "(x, y, z)" (spaces after the commas are ignored)
 * Constants are not allowed as parameters, only identifiers that only contain
 * alphabetical values
 *
 * @throw: throws if there is no outer-level pair of parentheses
 * or if any non-alphabetical chars are used in the identifier names
 * (no digits allowed either)
 *
 * @return: returns the identifiers inside the parentheses, the result should contain
 * all comma-separated values
 */
std::vector<std::string> parse_identifiers(std::string_view);
} // namespace worm::util::string

#endif // WORM_UTIL_STRING_H
