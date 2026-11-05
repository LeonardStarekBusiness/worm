#include "worm/wstd.h"

#include <iostream>

#include "worm/util/string.h"

namespace worm::wstd {
void print_n(const std::vector<std::string>& params) {
  for (const auto& str : params) {
    std::cout << util::string::strip_circumfix(str, '"');
  }
}

void println_n(const std::vector<std::string>& params) {
  for (const auto& str : params) {
    std::cout << util::string::strip_circumfix(str, '"') << '\n';
  }
}

void sleep_1(const std::vector<std::string>& params) {
  if (params.empty()) {

  }
}
} // namespace worm::wstd
