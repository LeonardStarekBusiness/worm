#ifndef WORM_FUNCTIONS_FUNCTIOR_ERROR_H
#define WORM_FUNCTIONS_FUNCTIOR_ERROR_H

#include <exception>
#include <string>

#include "worm/errors/err_info.h"

struct FunctionError : std::exception {
public:
  FunctionError(ErrInfo);

  const char* what() const noexcept override;

  ErrInfo err_info;

private:
  std::string extended_msg;
};

#endif // WORM_FUNCTIONS_FUNCTIOR_ERROR_H
