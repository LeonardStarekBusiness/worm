#include "worm/wstd.h"

#include <iostream>

#include "worm/util/string.h"

namespace worm::wstd {
void print(const std::vector<std::string>& params) {
  for (const auto& str : params) {
    std::cout << util::string::strip_circumfix(str, '"');
  }
}

void println(const std::vector<std::string>& params) {
  for (const auto& str : params) {
    std::cout << util::string::strip_circumfix(str, '"') << '\n';
  }
}
} // namespace worm::wstd
