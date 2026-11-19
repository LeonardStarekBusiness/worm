#ifndef WORM_PARSE_KEYWORDS_H
#define WORM_PARSE_KEYWORDS_H

#include <string_view>

namespace worm::parse::keywords {
constexpr std::string_view func_decl {"func"};
constexpr std::string_view condition {"if"};
/* It must hold that `is_identifier_char(vararg[0]) == true` */
constexpr std::string_view vararg {"vararg"};
} // namespace worm::parse::keywords

#endif // WORM_PARSE_KEYWORDS_H
