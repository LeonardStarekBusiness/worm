#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "worm/parse/parse.h"

namespace worm::util::string::parse_identifiers_tests {
void test_empty_input() {
  std::vector actual {parse::parse_identifiers("")};
  assert(actual.empty());
}

void test_non_ascii_input() {
  try {
    parse::parse_identifiers("hello, world, ü");
  } catch (std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_non_alphanumeric_space_input() {
  try {
    parse::parse_identifiers("yeye, +, hlkj");
  } catch (std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_normal_input() {
  std::vector expected {"hello", "world", "test"};
  std::vector actual {parse::parse_identifiers("(hello, world, test)")};

  assert(actual.size() == expected.size());
  for (std::size_t i {0}; i < actual.size(); ++i) {
    if (actual[i] != expected[i]) {
      std::cout << std::format(
        "\x1b[31mMismatch!\x1b[0m\tactual: '{}', expected: '{}'\n",
        actual[i],
        expected[i]
      );
      std::abort();
    }
  }
}

void test_normal_input_with_start_paren() {
  try {
    parse::parse_identifiers("(hello, world, test");
  } catch (std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_normal_input_with_end_paren() {
  try {
    parse::parse_identifiers("hello, world, test)");
  } catch (std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}
} // namespace worm::util::string::parse_identifiers_tests

int main() {
  worm::util::string::parse_identifiers_tests::test_empty_input();
  worm::util::string::parse_identifiers_tests::test_non_ascii_input();
  worm::util::string::parse_identifiers_tests::test_non_alphanumeric_space_input();
  worm::util::string::parse_identifiers_tests::test_normal_input();
  worm::util::string::parse_identifiers_tests::test_normal_input_with_start_paren();
  worm::util::string::parse_identifiers_tests::test_normal_input_with_end_paren();

  std::cout << "\x1b[32mAll tests for `worm::util::string` passed!\x1b[0m\n";
}
