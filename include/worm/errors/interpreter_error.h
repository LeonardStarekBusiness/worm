#ifndef WORM_ERRORS_INTERPRETER_ERROR_H
#define WORM_ERRORS_INTERPRETER_ERROR_H

#include <exception>
#include <string>

#include "worm/errors/err_info.h"

namespace worm::errors {
struct InterpreterError : public std::exception {
  InterpreterError(ErrInfo);

  const char* what() const noexcept override;
  InterpreterError shortened() const;

  ErrInfo err_info;

private:
  std::string extended_msg;
};
} // namespace worm::errors

#endif // WORM_ERRORS_INTERPRETER_ERROR_H
