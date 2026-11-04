#ifndef WORM_ERRORS_PARSE_ERROR_H
#define WORM_ERRORS_PARSE_ERROR_H

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

namespace worm::errors {
struct ParseError : public std::runtime_error {
  ParseError(
    const std::string& msg,
    std::size_t line_offset,
    const std::unique_ptr<std::runtime_error>& inner_err
  );

  std::size_t line_offset;
};
} // namespace worm::errors

#endif // WORM_ERRORS_PARSE_ERROR_H
