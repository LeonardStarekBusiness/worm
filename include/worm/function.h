#ifndef WORM_FUNCTION_H
#define WORM_FUNCTION_H

#include <string>
#include <vector>

template<typename T>
class Function {
private:
  std::vector<std::string> params;
  std::vector<Function> elementary_funcs;

public: 
  T execute() const;
};

#endif // WORM_FUNCTION_H
