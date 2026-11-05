#include "worm/parse/types.h"

#include <algorithm>

namespace worm::parse::types {
bool is_int(const std::string& s) {
  if (s.empty()) {
    return false;
  }

  if (s[0] == '-' && s.size() == 1) {
    return false;
  }

  return (s[0] == '-' || std::isdigit(s[0]))
    && std::all_of(s.begin() + 1, s.end(), [](auto ch) {
         return std::isdigit(ch);
       });
}
} // namespace utility::types
