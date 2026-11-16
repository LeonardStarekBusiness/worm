#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "worm/parse/parse.h"
#include "worm/util/string.h"

namespace worm::util::string {
namespace parse_identifiers_tests {
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

void test_input_with_missing_comma() {
  try {
    parse::parse_identifiers("(hello, world test)");
  } catch (std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}

void test_input_with_redundant_comma() {
  try {
    parse::parse_identifiers("(hello, world,, test)");
  } catch (std::invalid_argument& e) {
    return;
  }

  assert(false && "Expected to throw, but didn't!");
}
} // namespace parse_identifiers_tests

namespace contains_keyword_tests {
void test_empty_haystack() {
  assert(!contains_keyword("func", ""));
}

void test_slightly_small_keyword() {
  assert(!contains_keyword("func", "fun"));
}

void test_should_contain_keyword() {
  assert(contains_keyword("func", "hello this is a cool func to test!"));
}

void test_keyword_at_start() {
  assert(contains_keyword("func", "func this is a cool hello to test!"));
}

void test_keyword_at_end() {
  assert(contains_keyword("func", "test! this is a cool hello to func"));
}

void test_keyword_has_adjacent_char_at_tail() {
  assert(!contains_keyword("func", "hello this is a cool funct to test!"));
}

void test_keyword_has_adjacent_char_at_head() {
  assert(!contains_keyword("func", "hello this is a cool ffunc to test!"));
}

void test_keyword_has_adjacent_char_surrounding() {
  assert(!contains_keyword("func", "hello this is a cool ffunction to test!"));
}

void test_keyword_is_in_string() {
  assert(!contains_keyword("func", "hello this is \"a cool func to test\"!"));
}

void test_haystack_has_many_strings() {
  assert(contains_keyword("func", "\"hello this\" is \"a cool func to test\" func!"));
}
} // namespace contains_keyword_tests
} // namespace worm::util::string

using namespace worm::util::string;

int main() {
  parse_identifiers_tests::test_empty_input();
  parse_identifiers_tests::test_non_ascii_input();
  parse_identifiers_tests::test_non_alphanumeric_space_input();
  parse_identifiers_tests::test_normal_input();
  parse_identifiers_tests::test_normal_input_with_start_paren();
  parse_identifiers_tests::test_normal_input_with_end_paren();
  parse_identifiers_tests::test_input_with_missing_comma();

  contains_keyword_tests::test_empty_haystack();
  contains_keyword_tests::test_slightly_small_keyword();
  contains_keyword_tests::test_should_contain_keyword();
  contains_keyword_tests::test_keyword_at_start();
  contains_keyword_tests::test_keyword_at_end();
  contains_keyword_tests::test_keyword_has_adjacent_char_at_tail();
  contains_keyword_tests::test_keyword_has_adjacent_char_at_head();
  contains_keyword_tests::test_keyword_has_adjacent_char_surrounding();
  contains_keyword_tests::test_keyword_is_in_string();

  std::cout << "\x1b[32mAll tests for `worm::util::string` passed!\x1b[0m\n";
}
