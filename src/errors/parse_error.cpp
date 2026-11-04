#include "worm/parse/parse_error.h"

#include <exception>
#include <string>

#include "worm/errors/err_info.h"

namespace worm::parse {
namespace {
std::string extend_msg(const std::string& msg, std::exception_ptr err_ptr) {
  if (!err_ptr) {
    return msg;
  }

  std::string ret {msg + "\n"};
  try {
    std::rethrow_exception(err_ptr);
  } catch (const std::exception& e) {
    return msg + e.what();
  } catch (...) {
    return msg + "Unknown exception!";
  }
}
} // namespace

ParseError::ParseError(ErrInfo err_info)
    : err_info {err_info}
    , extended_msg {extend_msg(err_info.msg, err_info.og_err)} {}

const char* ParseError::what() const noexcept {
  return extended_msg.c_str();
}
} // namespace worm::parse
