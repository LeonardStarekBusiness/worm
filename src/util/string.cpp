#include "worm/util/string.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "worm/parse/parse.h"

namespace worm::util::string {
bool isspace(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isspace(uch);
}

void trim(std::string& s, bool (*should_strip)(char)) {
  std::size_t start {0};
  while (start < s.size() && should_strip(s[start])) {
    ++start;
  }

  if (start >= s.size()) {
    s.clear();
    return;
  }

  std::size_t end_incl {s.size()};
  while (end_incl-- > 0 && should_strip(s[end_incl])) {
    s.pop_back();
  }

  s.erase(0, start);
}

std::string_view trimmed_view(const std::string& s) {
  auto start {std::find_if(s.begin(), s.end(), [](auto ch) {
    return !isspace(ch);
  })};

  auto end_incl {std::find_if(s.rbegin(), s.rend(), [](auto ch) {
    return !isspace(ch);
  })};

  return std::string_view {start, end_incl.base()};
}

std::string_view strip_circumfix(const std::string& s, char target) {
  if (s.empty() || s.front() != target || s.back() != target) {
    return s;
  }

  return {s.begin() + 1, s.end() - 1};
}

std::string_view trimmed_view(std::string_view sv) {
  auto start {std::find_if(sv.begin(), sv.end(), [](auto ch) {
    return !isspace(ch);
  })};

  auto end_incl {std::find_if(sv.rbegin(), sv.rend(), [](auto ch) {
    return !isspace(ch);
  })};

  return std::string_view {start, end_incl.base()};
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

    bool comma_separated {false};
    while (ilen < sv.size() && !is_token_char(sv[ilen])) {
      if (comma_separated && sv[ilen] == ',') {
        throw std::invalid_argument("Error parsing list: redundant comma!");
      }

      comma_separated |= sv[ilen] == ',';
      ++ilen;
    }

    if (ilen < sv.size() && !comma_separated) {
      throw std::invalid_argument("Error parsing list: missing comma!");
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

std::size_t indent_lvl(const std::string& s) {
  for (std::size_t i {0}; i < s.size(); ++i) {
    if (!isspace(s[i])) {
      return i;
    }
  }

  return s.size();
}
} // namespace worm::util::string
