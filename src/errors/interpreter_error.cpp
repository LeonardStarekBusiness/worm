#include "worm/errors/interpreter_error.h"

#include "worm/errors/err_info.h"

namespace worm::errors {
namespace {
std::string extend_msg(const std::string& msg, std::exception_ptr err_ptr) {
  if (!err_ptr) {
    return msg;
  }

  std::string ret {msg + "\n"};
  try {
    std::rethrow_exception(err_ptr);
  } catch (const std::exception& e) {
    return ret + e.what();
  } catch (...) {
    return ret + "Unknown exception!";
  }
}
} // namespace

InterpreterError::InterpreterError(ErrInfo err_info)
    : err_info {err_info}
    , extended_msg {extend_msg(err_info.msg, err_info.og_err)} {}

const char* InterpreterError::what() const noexcept {
  return extended_msg.c_str();
}
} // namespace worm::errors
