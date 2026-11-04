#ifndef WORM_ERRORS_ERR_INFO_H
#define WORM_ERRORS_ERR_INFO_H

#include <cstddef>
#include <exception>
#include <string>

struct ErrInfo {
  std::string msg;
  std::size_t line_offset;
  std::exception_ptr og_err;
};

#endif // WORM_ERRORS_ERR_INFO_H
