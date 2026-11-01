#ifndef WORM_PARSE_PARSE_H
#define WORM_PARSE_PARSE_H

namespace worm::parse {
constexpr char str_delim {'"'};
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
} // namespace worm::parse

#endif // WORM_PARSE_PARSE_H
