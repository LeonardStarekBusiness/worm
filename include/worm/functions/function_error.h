#ifndef WORM_FUNCTIONS_FUNCTIOR_ERROR_H
#define WORM_FUNCTIONS_FUNCTIOR_ERROR_H

#include "worm/errors/err_info.h"
#include "worm/errors/interpreter_error.h"

namespace worm::functions {
struct FunctionError : errors::InterpreterError {
  FunctionError(errors::ErrInfo err_info) : errors::InterpreterError {err_info} {}
};
} // namespace worm::functions

#endif // WORM_FUNCTIONS_FUNCTIOR_ERROR_H
