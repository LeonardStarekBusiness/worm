#include "worm/parse/parse.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string_view>
#include <vector>

#include "worm/parse/construct.h"
#include "worm/parse/keywords.h"
#include "worm/parse/types.h"
#include "worm/util/string.h"

namespace worm::parse {
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

  if (sv[0] == types::str_delim) {
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

Construct determine_construct(std::string_view line) {
  if (line.empty()) {
    return Construct::Statement;
  }

  static const std::pair<const std::string_view, const Construct> map[] {
    {keywords::condition, Construct::Condition},
    {keywords::func_decl, Construct::FuncDecl}
  };

  for (const auto& [keyword, construct] : map) {
    if (util::string::contains_keyword(keyword, line)) {
      return construct;
    }
  }

  return Construct::Statement;
}

bool is_identifier_char(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isalpha(uch) || uch == '_';
}

bool is_signature_char(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isspace(uch) || std::isalpha(uch) || uch == ',';
}

bool is_numeric_char(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isdigit(uch) || uch == '.';
}

std::vector<std::string> parse_identifiers(std::string_view sv) {
  if (sv.empty()) {
    return {};
  }

  if (sv.front() != '(' || sv.back() != ')') {
    throw std::invalid_argument(
      std::string {"Error parsing parameters: missing outer parentheses!\n"}
      + "Input: \x1b[31m" + std::string {sv} + "\x1b[0m"
    );
  }

  sv = {sv.begin() + 1, sv.end() - 1};
  bool invalid_chars {!std::all_of(sv.begin(), sv.end(), parse::is_signature_char)};

  if (invalid_chars) {
    throw std::invalid_argument(
      std::string {"Error parsing parameters: contains invalid chars!\n"}
      + "Input: \x1b[31m" + std::string {sv} + "\x1b[0m"
    );
  }

  return util::string::parse_list(sv, identifier_len, parse::is_identifier_char);
}

std::vector<std::string> parse_params(std::string_view sv) {
  if (sv.empty()) {
    return {};
  }

  if (sv.front() != '(' || sv.back() != ')') {
    throw std::invalid_argument(
      std::string {"Error parsing parameters: missing outer parentheses!\n"}
      + "Input: \x1b[31m" + std::string {sv} + "\x1b[0m"
    );
  }

  sv = {sv.begin() + 1, sv.end() - 1};
  return util::string::parse_list(sv, param_len, [](auto ch) {
    return parse::is_identifier_char(ch) || parse::is_numeric_char(ch)
      || ch == parse::str_delim;
  });
}
} // namespace worm::parse
