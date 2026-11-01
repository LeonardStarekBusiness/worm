#include "worm/util/string.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "worm/parse/parse.h"

namespace worm::util::string {
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

std::vector<std::string> parse_list(
  std::string_view sv,
  std::size_t (*token_length)(std::string_view),
  bool (*is_token_char)(char)
) {
  std::vector<std::string> params;
  while (!sv.empty()) {
    std::size_t ilen {token_length(sv)};
    std::string param {sv.begin(), sv.begin() + ilen};
    params.push_back(param);

    while (ilen < sv.size() && !is_token_char(sv[ilen])) {
      ++ilen;
    }

    sv = {sv.begin() + ilen, sv.end()};
  }

  return params;
}

bool contains_keyword(std::string_view needle, std::string_view haystack) {
  if (haystack.size() < needle.size()) {
    return false;
  }

  if (
    std::string_view {haystack.begin(), haystack.begin() + needle.size()} == needle
    && (haystack.size() == needle.size() || std::isspace(haystack[needle.size()]))
  ) {
    return true;
  }

  bool in_str {false};
  std::size_t idx {1};

  while (idx + needle.size() < haystack.size()) {
    if (haystack[idx] == parse::str_delim) {
      in_str = !in_str;
    }

    if (
      in_str || !std::isspace(haystack[idx - 1])
      || !std::isspace(haystack[idx + needle.size()])
    ) {
      ++idx;
      continue;
    }

    std::string_view cmp {
      haystack.begin() + idx,
      haystack.begin() + idx + needle.size()
    };

    if (cmp == needle) {
      return true;
    }

    ++idx;
  }

  return needle == std::string_view {haystack.begin() + idx, haystack.end()};
}
} // namespace worm::util::string
