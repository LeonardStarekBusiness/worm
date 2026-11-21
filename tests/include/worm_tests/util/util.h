#ifndef WORM_TESTS_UTIL_UTIL_H
#define WORM_TESTS_UTIL_UTIL_H

#include <cstdlib>
#include <format>
#include <iostream>
#include <vector>

namespace worm_tests::util {
template <typename T>
void cmp_vecs(std::vector<T> actual, std::vector<T> expected) {
  if (actual.size() != expected.size()) {
    std::cout << std::format(
      "\x1b[31mMismatch!\x1b[0m\tactual size: {}, expected size: {}\n",
      actual.size(),
      expected.size()
    );
    std::abort();
  }

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
} // namespace worm_tests::util

#endif // WORM_TESTS_UTIL_UTIL_H
