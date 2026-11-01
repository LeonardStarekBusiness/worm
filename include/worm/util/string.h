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
} // namespace worm::util::string

#endif // WORM_UTIL_STRING_H
