#ifndef WORM_ERRORS_ERR_INFO_H
#define WORM_ERRORS_ERR_INFO_H

#include <cstddef>
#include <exception>
#include <string>

namespace worm::errors {
struct ErrInfo {
  std::string msg;
  std::size_t line_offset = 0;
  std::exception_ptr og_err = nullptr;
};
} // namespace worm::errors

#endif // WORM_ERRORS_ERR_INFO_H
