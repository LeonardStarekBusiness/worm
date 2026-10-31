#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "worm/util/string.h"

namespace util::string::parse_params_tests {
void test_empty_input() {
  std::vector actual {parse_params("")};
  assert(actual.empty());
}

void test_non_ascii_input() {
  try {
    parse_params("hello, world, ü");
  } catch (std::runtime_error& e) {
    assert(
      e.what() == std::string("Error parsing parameters: contains invalid chars!")
    );
  }
}

void test_non_alphanumeric_space_input() {
  try {
    parse_params("yeye, +, hlkj");
  } catch (std::runtime_error& e) {
    assert(
      e.what() == std::string("Error parsing parameters: contains invalid chars!")
    );
  }
}

void test_normal_input() {
  std::vector expected {"hello", "world", "test"};
  std::vector actual {parse_params("(hello, world, test)")};

  assert(actual.size() == expected.size());
  for (std::size_t i {0}; i < actual.size(); ++i) {
    if (actual[i] != expected[i]) {
      std::cout << std::string("actual: ") + actual[i] + ", expected: " + expected[i];
      std::abort();
    }
  }
}

void test_normal_input_with_start_paren() {
  try {
    parse_params("(hello, world, test");
  } catch (std::runtime_error& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_normal_input_with_end_paren() {
  try {
    parse_params("hello, world, test)");
  } catch (std::runtime_error& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}
} // namespace util::string::parse_params_tests

int main() {
  util::string::parse_params_tests::test_empty_input();
  util::string::parse_params_tests::test_non_ascii_input();
  util::string::parse_params_tests::test_non_alphanumeric_space_input();
  util::string::parse_params_tests::test_normal_input();
  util::string::parse_params_tests::test_normal_input_with_start_paren();
  util::string::parse_params_tests::test_normal_input_with_end_paren();
}
