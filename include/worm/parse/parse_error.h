#ifndef WORM_PARSE_PARSE_ERROR_H
#define WORM_PARSE_PARSE_ERROR_H

#include <exception>

#include "worm/errors/err_info.h"

namespace worm::parse {
struct ParseError : public std::exception {
  ParseError(ErrInfo);

  const char* what() const noexcept override;

  ErrInfo err_info;

private:
  std::string extended_msg;
};
} // namespace worm::parse

#endif // WORM_PARSE_PARSE_ERROR_H
