#include "worm/util/string.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace util::string {
void trim(std::string& s) {
  std::size_t start {0};
  while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
    ++start;
  }

  if (start >= s.size()) {
    s.clear();
    return;
  }

  std::size_t end_incl {s.size()};
  while (end_incl-- > 0 && std::isspace(static_cast<unsigned char>(s[end_incl]))) {
    s.pop_back();
  }

  s.erase(0, start);
}

std::vector<std::string> parse_params(std::string_view s) {
  if (s.empty()) {
    return {};
  }

  std::string::difference_type start_offset {s[0] == '(' ? 1 : 0};
  std::string::difference_type end_offset {s[s.size() - 1] == ')' ? 1 : 0};

  bool invalid_chars {
    !std::all_of(s.begin() + start_offset, s.end() - end_offset, [](auto ch) {
      auto uch {static_cast<unsigned char>(ch)};
      return std::isalpha(uch) || std::isspace(uch) || uch == ',';
    })
  };

  if (invalid_chars) {
    throw std::runtime_error("Error parsing parameters: contains invalid chars!");
  }

  std::vector<std::string> params;
  std::size_t start {0};
  std::size_t end {start};

  while (start < s.size()) {
    while (end < s.size() && std::isalpha(static_cast<unsigned char>(s[end]))) {
      ++end;
    }

    std::string param {
      s.begin() + static_cast<std::string::difference_type>(start),
      s.begin() + static_cast<std::string::difference_type>(end)
    };
    params.push_back(param);

    while (end < s.size() && !std::isalpha(static_cast<unsigned char>(s[end]))) {
      ++end;
    }

    start = end;
    ++end;
  }

  return params;
}
} // namespace util::string
