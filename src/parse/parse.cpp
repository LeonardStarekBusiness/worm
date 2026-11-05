#include "worm/parse/parse.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string_view>
#include <vector>

#include "worm/errors/err_info.h"
#include "worm/parse/construct.h"
#include "worm/parse/keywords.h"
#include "worm/parse/parse_error.h"
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

std::size_t number_len(std::string_view sv) {
  std::size_t len {0};
  while (len < sv.size() && parse::is_numeric_char(sv[len])) {
    ++len;
  }

  return len;
}

std::size_t string_len(std::string_view sv) {
  if (sv.empty()) {
    return 0;
  }

  bool in_str {sv[0] == parse::types::str_delim};
  std::size_t len {1};

  while (in_str && len < sv.size()) {
    if (sv[len - 1] != '\\' && sv[len] == parse::types::str_delim) {
      in_str = false;
    }

    ++len;
  }

  return len;
}

std::size_t param_len(std::string_view sv) {
  if (sv.empty()) {
    return 0;
  }

  if (parse::is_identifier_char(sv[0])) {
    return identifier_len(sv);
  } else if (parse::is_numeric_char(sv[0])) {
    return number_len(sv);
  } else if (sv[0] == parse::types::str_delim) {
    return string_len(sv);
  } else {
    return 0;
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
    throw ParseError {errors::ErrInfo {
      .msg {
        std::string {"Error parsing parameters: missing outer parentheses!\n"}
        + "Input: \x1b[31m" + std::string {sv} + "\x1b[0m"
      },
    }};
  }

  sv = {sv.begin() + 1, sv.end() - 1};
  bool invalid_chars {!std::all_of(sv.begin(), sv.end(), parse::is_signature_char)};

  if (invalid_chars) {
    throw ParseError {errors::ErrInfo {
      .msg {
        std::string {"Error parsing identifiers: contains invalid chars!\n"}
        + "Input: \x1b[31m" + std::string {sv} + "\x1b[0m"
      },
    }};
  }

  return util::string::parse_list(sv, identifier_len, parse::is_identifier_char);
}

std::vector<std::string> parse_params(std::string_view sv) {
  if (sv.empty()) {
    return {};
  }

  if (sv.front() != '(' || sv.back() != ')') {
    throw ParseError {errors::ErrInfo {
      .msg {
        std::string {"Error parsing parameters: missing outer parentheses!\n"}
        + "Input: \x1b[31m" + std::string {sv} + "\x1b[0m"
      },
    }};
  }

  sv = {sv.begin() + 1, sv.end() - 1};
  return util::string::parse_list(sv, param_len, [](auto ch) {
    return parse::is_identifier_char(ch) || parse::is_numeric_char(ch)
      || ch == parse::str_delim;
  });
}
} // namespace worm::parse
