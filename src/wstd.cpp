#include "worm/wstd.h"

#include <iostream>

namespace wstd {
void print(const std::vector<std::string>& params) {
  for (const auto& str : params) {
    std::cout << str;
  }
}

void println(const std::vector<std::string>& params) {
  for (const auto& str : params) {
    std::cout << str;
  }

  std::cout << '\n';
}
} // namespace wstd
