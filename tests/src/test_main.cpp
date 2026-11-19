#include <iostream>
#include <string_view>

#include "worm_tests/test_parse.h"
#include "worm_tests/util_tests/test_string.h"

int main() {
  std::pair<std::string_view, void(*)()> tests[] {
    {"`worm::parse`", worm::parse::test_parse},
    {"`worm::util::string`", worm::util::string::test_util_string},
  };

  for (const auto& [name, test] : tests) {
    std::cout << "\nTesting " << name << "... "; 
    std::cout.flush();
    test();
    std::cout << "\x1b[32mpass\x1b[0m";
  }

  std::cout << "\n\n\x1b[1;32mALL TESTS PASSED!\x1b[0m\n" << std::endl;
  return 0;
}
