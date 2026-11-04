#include "worm/errors/parse_error.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

namespace worm::errors {
namespace {
std::string extended_msg(
  const std::string& msg,
  const std::unique_ptr<std::runtime_error>& err
) {
  return msg + (err ? std::string {"\n"} + err->what() : "");
}
} // namespace

ParseError::ParseError(
  const std::string& msg,
  std::size_t line_offset,
  const std::unique_ptr<std::runtime_error>& inner_err
)
    : std::runtime_error(extended_msg(msg, inner_err))
    , line_offset {line_offset} {}
} // namespace worm::errors
