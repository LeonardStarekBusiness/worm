#ifndef WORM_PARSE_PARSE_H
#define WORM_PARSE_PARSE_H

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "worm/parse/construct.h"

namespace worm::parse {
constexpr char str_delim {'"'};
/*
 * Reads the given line and determines the exact construct of the line
 *
 * Edge cases: 
 * Should ignore empty lines
 * 
 * @return: Returns an enum type that specifies the type of construct; it's value is
 * intended to be used at the callsite to choose how the parser should proceed
 */
Construct determine_construct(std::string_view);
/*
 * @return: Returns `true` if given char can legally appear in
 * a .worm function signature, otherwise returns `false`
 */
bool is_signature_char(char);
/*
 * @return: Returns `true` if given char can legally belong to an
 * identifier in .worm
 */
bool is_identifier_char(char);
/*
 * @return: Returns `true` if given char holds a numeric semantic meaning
 * (characters needed to represent ints/floats in .worm)
 */
bool is_numeric_char(char);
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
/*
 * Parses parameters like `worm::parse::parse_identifiers()`, just that
 * it also allows for string literals (expressions wrapped in `parse::str_delim`)
 * and numeric constants (like 42.67, 69)
 */
std::vector<std::string> parse_params(std::string_view);
/*
 * Counts the amount of chars from the beginning of the string view that belong to a
 * parameter that starts from the start of the string view
 *
 * A parameter can be an identifier, a string constant, a numeric constant
 *
 * Example: 
 * "str", 45, this_is_the_next_identifier" -> param_len() would be 5
 */
std::size_t param_len(std::string_view);
/*
 * Counts the amount of chars from the beginning of the string view that belong to the
 * next identifier
 *
 * Example: 
 * xyz, this_is_the_next_identifier -> `identifier_len()` would be 3
 */
std::size_t identifier_len(std::string_view);
/*
 * Counts the amount of chars from the beginning of the string view that belong to the
 * next numeric argument
 *
 * Example: 
 * 5454.0545, this_is_the_next_identifier -> `number_len()` would be 9
 */
std::size_t number_len(std::string_view);
/*
 * Counts the amount of chars from the beginning of the string view that belong to a
 * string parameter 
 * !INCLUDING! the wrapping string delimiters `worm::parse::types::str_delim`
 *
 * Example: 
 * "xyz", this_is_the_next_identifier -> `string_len()` would be 5
 */
std::size_t string_len(std::string_view);
} // namespace worm::parse
#endif // WORM_PARSE_PARSE_H
