#include "worm/wstd.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "worm/errors/err_info.h"
#include "worm/functions/function_error.h"
#include "worm/parse/types.h"
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

void sleep_ms_1(const std::vector<std::string>& params) {
  if (params.empty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(0));
  }

  if (!parse::types::is_int(params[0])) {
    throw functions::FunctionError {errors::ErrInfo {
      .msg {"Error executing `worm::wstd::sleep_1()`: expects an integer parameter!"}
    }};
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(std::stoi(params[0])));
}
} // namespace worm::wstd
