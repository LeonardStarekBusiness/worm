#ifndef WORM_UTIL_STRING_H
#define WORM_UTIL_STRING_H

#include <concepts>
#include <cstddef>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace worm::util::string {
template <typename L>
concept LengthGetter = requires(L lg, std::string_view sv) {
  { lg(sv) } -> std::same_as<std::size_t>;
};

[[nodiscard]] bool isspace(char ch);
/*
 * Removes leading or trailing whitespaces
 */
void trim(std::string&, bool (*should_strip)(char) = isspace);
/*
 * Provides a view into the input string(-view), but without the leading or
 * trailing whitespaces
 */
[[nodiscard]] std::string_view trimmed_view(const std::string&);
[[nodiscard]] std::string_view trimmed_view(std::string_view);
/*
 * Provides a view without one layer of wrapping @param target
 * If the provided string is not wrapped at the front AND back with @param target, 
 * this function returns a view of the unchanged input string 
 */
[[nodiscard]] std::string_view strip_circumfix(const std::string&, char target);
/*
 * Parses a comma-separated list according to @param token_length and
 * @param is_token_char
 *
 * @param: Takes a string_view as the buffer to parse, additionally to:
 * @param token_length: main engine to determine how long a token is
 * @param is_token_char: predicate that provides the knowledge whether a char
 * is the start of a token or can be skipped
 */
template <LengthGetter L>
[[nodiscard]] std::vector<std::string> parse_list(
  std::string_view sv,
  L&& token_length,
  bool (*is_token_char)(char)
) {
  std::vector<std::string> params;
  sv = trimmed_view(sv);

  while (!sv.empty()) {
    std::string sv_cp {sv};
    std::size_t token_len {token_length(sv)};
    std::string param {sv.begin(), sv.begin() + token_len};
    params.push_back(param);

    bool comma_separated {false};
    while (token_len < sv.size() && !is_token_char(sv[token_len])) {
      if (comma_separated && sv[token_len] == ',') {
        throw std::invalid_argument(
          std::format(
            "Operating on input \"\x1b[31m{}\x1b[0m\":\n"
            "Error in `worm::util::string::parse_list()`: redundant comma!",
            sv_cp
          )
        );
      }

      comma_separated |= sv[token_len] == ',';
      ++token_len;
    }

    if (token_len < sv.size() && !comma_separated) {
      throw std::invalid_argument(
        std::format(
          "Operating on input \"\x1b[31m{}\x1b[0m\":\n"
          "Error in `worm::util::string::parse_list()`: missing comma!",
          sv_cp
        )
      );
    }

    sv = {sv.begin() + token_len, sv.end()};
  }

  return params;
}
/*
 * Checks the "haystack" for the "needle".
 * Note that the needle is not allowed to be inside string delimiters
 * and that it must be surrounded by whitespaces to reduce ambiguity
 */
[[nodiscard]] bool contains_keyword(std::string_view needle, std::string_view haystack);
/*
 * Determines the level of indentation of given line
 */
[[nodiscard]] std::size_t indent_lvl(const std::string&);
/*
 * Removes all escaping of string delimiters
 *
 * Example:
 * Input: Hello, \" World
 * Output: Hello, " World
 */
void unescape_str_delims(std::string&);
} // namespace worm::util::string

#endif // WORM_UTIL_STRING_H
