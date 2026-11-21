#ifndef WORM_PARSE_PARSE_ERROR_H
#define WORM_PARSE_PARSE_ERROR_H

#include "worm/errors/err_info.h"
#include "worm/errors/interpreter_error.h"

namespace worm::parse {
struct ParseError : public errors::InterpreterError {
  ParseError(errors::ErrInfo err_info) : errors::InterpreterError {err_info} {}
};
} // namespace worm::parse

#endif // WORM_PARSE_PARSE_ERROR_H
