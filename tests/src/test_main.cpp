#include <iostream>

#include "worm_tests/test_parse.h"
#include "worm_tests/util_tests/test_string.h"

int main() {
  worm::parse::test_parse();
  worm::util::string::test_util_string();

  std::cout << "\x1b[1;32mALL TESTS PASSED!\x1b[0m" << std::endl;

  return 0;
}
