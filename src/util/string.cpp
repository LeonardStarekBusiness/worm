#include "worm/util/string.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "worm/parse/parse.h"
#include "worm/parse/types.h"

namespace worm::util::string {
namespace {
std::size_t identifier_len(std::string_view sv) {
  std::size_t len {0};
  while (len < sv.size() && parse::is_identifier_char(sv[len])) {
    ++len;
  }

  return len;
}

std::size_t param_len(std::string_view sv) {
  if (sv.empty()) {
    return 0;
  }

  bool in_str {false};
  bool escaped {false};
  bool (*allowed_char)(char);

  if (sv[0] == parse::types::str_delim) {
    allowed_char = [](char) {
      return true;
    };

    in_str = true;
  } else if (parse::is_identifier_char(sv[0])) {
    return identifier_len(sv);
  } else if (parse::is_numeric_char(sv[0])) {
    allowed_char = parse::is_numeric_char;
  } else {
    return 0;
  }

  std::size_t len {1};
  while (
    len < sv.size() && allowed_char(sv[len])
    && (!in_str || !escaped || sv[len] != parse::types::str_delim)
  ) {
    if (in_str) {
      escaped = !escaped && sv[len] == '\\';
    }

    ++len;
  }

  if (len < sv.size() && in_str && sv[len] == parse::types::str_delim) {
    return ++len;
  } else {
    return len;
  }
}
} // namespace

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

std::vector<std::string> parse_identifiers(std::string_view sv) {
  if (sv.empty()) {
    return {};
  }

  if (sv.front() != '(' || sv.back() != ')') {
    throw std::invalid_argument("Error parsing parameters: missing outer parentheses!");
  }

  sv = {sv.begin() + 1, sv.end() - 1};
  bool invalid_chars {!std::all_of(sv.begin(), sv.end(), parse::is_signature_char)};

  if (invalid_chars) {
    throw std::invalid_argument("Error parsing parameters: contains invalid chars!");
  }

  std::vector<std::string> params;
  while (!sv.empty()) {
    std::size_t ilen {identifier_len(sv)};
    std::string param {sv.begin(), sv.begin() + ilen};
    params.push_back(param);

    while (ilen < sv.size() && !parse::is_identifier_char(sv[ilen])) {
      ++ilen;
    }

    sv = {sv.begin() + ilen, sv.end()};
  }

  return params;
}
} // namespace worm::util::string
