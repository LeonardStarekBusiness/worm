#include <cassert>
#include <iostream>
#include <string>

#include "worm/util/string.h"

namespace worm::util::string {
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

namespace unescape_str_delims_tests {
void test() {
  std::string expected {"Hello, \" World"};
  std::string actual {"Hello, \\\" World"};
  unescape_str_delims(actual);

  assert(actual == expected);
}
}

void test_util_string() {
  contains_keyword_tests::test_empty_haystack();
  contains_keyword_tests::test_slightly_small_keyword();
  contains_keyword_tests::test_should_contain_keyword();
  contains_keyword_tests::test_keyword_at_start();
  contains_keyword_tests::test_keyword_at_end();
  contains_keyword_tests::test_keyword_has_adjacent_char_at_tail();
  contains_keyword_tests::test_keyword_has_adjacent_char_at_head();
  contains_keyword_tests::test_keyword_has_adjacent_char_surrounding();
  contains_keyword_tests::test_keyword_is_in_string();

  unescape_str_delims_tests::test();

  std::cout << "\x1b[32mAll tests for `worm::util::string` passed!\x1b[0m\n";
}
} // namespace worm::util::string
