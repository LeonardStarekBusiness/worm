#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include "worm/errors/interpreter_error.h"
#include "worm/parse/keywords.h"
#include "worm/parse/parse.h"
#include "worm/parse/parse_error.h"
#include "worm_tests/util/util.h"

namespace worm::parse {
namespace parse_identifiers_tests {
void test_empty_input() {
  std::vector actual {parse::parse_identifiers("")};
  assert(actual.empty());
}

void test_non_ascii_input() {
  try {
    static_cast<void>(parse::parse_identifiers("hello, world, ü"));
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_non_alphanumeric_space_input() {
  try {
    static_cast<void>(parse::parse_identifiers("yeye, +, hlkj"));
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_normal_input() {
  std::vector<std::string> expected {"hello", "world", "test"};
  std::vector actual {parse::parse_identifiers("(hello, world, test)")};

  worm_tests::util::cmp_vecs(actual, expected);
}

void test_normal_input_with_start_paren() {
  try {
    static_cast<void>(parse::parse_identifiers("(hello, world, test"));
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_normal_input_with_end_paren() {
  try {
    static_cast<void>(parse::parse_identifiers("hello, world, test)"));
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_input_with_missing_comma() {
  try {
    static_cast<void>(parse::parse_identifiers("(hello, world test)"));
  } catch (const std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_input_with_redundant_comma() {
  try {
    static_cast<void>(parse::parse_identifiers("(hello, world,, test)"));
  } catch (const std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}
} // namespace parse_identifiers_tests

namespace vararg_len_tests {
void test_normal_input() {
  std::pair expected {keywords::vararg.size() + 2 + 1, 5};
  auto actual {vararg_len("vararg[5], next_param")};
  assert(actual == expected);
}

void test_longer_idx() {
  std::pair expected {keywords::vararg.size() + 2 + 3, 128};
  auto actual {vararg_len("vararg[128], next_param")};
  assert(actual == expected);
}

void test_float_idx_fails() {
  try {
    static_cast<void>(vararg_len("vararg[5.1], next_param"));
  } catch (const errors::InterpreterError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_missing_right_bracket() {
  try {
    static_cast<void>(vararg_len("vararg[5, next_param"));
  } catch (const errors::InterpreterError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_missing_left_bracket() {
  try {
    static_cast<void>(vararg_len("vararg5], next_param"));
  } catch (const errors::InterpreterError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}
} // namespace vararg_len_tests

void test_parse() {
  parse_identifiers_tests::test_empty_input();
  parse_identifiers_tests::test_non_ascii_input();
  parse_identifiers_tests::test_non_alphanumeric_space_input();
  parse_identifiers_tests::test_normal_input();
  parse_identifiers_tests::test_normal_input_with_start_paren();
  parse_identifiers_tests::test_normal_input_with_end_paren();
  parse_identifiers_tests::test_input_with_missing_comma();
  parse_identifiers_tests::test_input_with_redundant_comma();

  vararg_len_tests::test_normal_input();
  vararg_len_tests::test_longer_idx();
  vararg_len_tests::test_float_idx_fails();
  vararg_len_tests::test_missing_left_bracket();
  vararg_len_tests::test_missing_right_bracket();
}
} // namespace worm::parse
