#include "worm/parse/parse.h"

#include <cctype>

namespace worm::parse {
bool is_identifier_char(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isalpha(uch);
}

bool is_signature_char(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isspace(uch) || std::isalpha(uch) || uch == ',';
}

bool is_numeric_char(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isdigit(uch) || uch == '.';
}
} // namespace worm::parse
