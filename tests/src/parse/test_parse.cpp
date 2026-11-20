#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
    parse::parse_identifiers("hello, world, ü");
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_non_alphanumeric_space_input() {
  try {
    parse::parse_identifiers("yeye, +, hlkj");
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
    parse::parse_identifiers("(hello, world, test");
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_normal_input_with_end_paren() {
  try {
    parse::parse_identifiers("hello, world, test)");
  } catch (const ParseError& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_input_with_missing_comma() {
  try {
    parse::parse_identifiers("(hello, world test)");
  } catch (const std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_input_with_redundant_comma() {
  try {
    parse::parse_identifiers("(hello, world,, test)");
  } catch (const std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}
} // namespace parse_identifiers_tests

void test_parse() {
  parse_identifiers_tests::test_empty_input();
  parse_identifiers_tests::test_non_ascii_input();
  parse_identifiers_tests::test_non_alphanumeric_space_input();
  parse_identifiers_tests::test_normal_input();
  parse_identifiers_tests::test_normal_input_with_start_paren();
  parse_identifiers_tests::test_normal_input_with_end_paren();
  parse_identifiers_tests::test_input_with_missing_comma();
  parse_identifiers_tests::test_input_with_redundant_comma();

  std::cout << "\x1b[32mAll tests for `worm::parse` passed!\x1b[0m\n";
}
} // namespace worm::parse
