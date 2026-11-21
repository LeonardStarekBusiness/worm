#ifndef WORM_PARSE_TYPES_H
#define WORM_PARSE_TYPES_H

#include <string>

namespace worm::parse::types {
/* char value that .worm source code should use to wrap strings in */
constexpr char str_delim {'"'};
bool is_int(const std::string& s);
} // namespace worm::parse::types

#endif // WORM_PARSE_TYPES_H
