#ifndef WORM_UTIL_STRING_H
#define WORM_UTIL_STRING_H

#include <string>
#include <string_view>
#include <vector>

namespace worm::util::string {
bool isspace(char ch);
/*
 * Removes leading or trailing whitespaces
 */
void trim(std::string&);
/*
 * Provides a view into the input string, but without the leading or
 * trailing whitespaces
 */
std::string_view trimmed_view(const std::string&);
/*
 * Parses a comma-separated list according to @param token_length and
 * @param is_token_char
 *
 * @param: Takes a string_view as the buffer to parse, additionally to:
 * @param token_length: main engine to determine how long a token is, starting
 * from the first char of that token
 * @param is_token_char: predicate that provides the knowledge whether a char
 * is the start of a token or can be skipped
 */
std::vector<std::string> parse_list(
  std::string_view,
  std::size_t (*token_length)(std::string_view),
  bool (*is_token_char)(char)
);
/*
 * Checks the "haystack" for the "needle".
 * Note that the needle is not allowed to be inside string delimiters
 * and that it must be surrounded by whitespaces to reduce ambiguity
 */
bool contains_keyword(std::string_view needle, std::string_view haystack);
std::size_t indent_lvl(const std::string&);
} // namespace worm::util::string

#endif // WORM_UTIL_STRING_H
