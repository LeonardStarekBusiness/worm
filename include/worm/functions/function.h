#ifndef WORM_FUNCTIONS_FUNCTION_H
#define WORM_FUNCTIONS_FUNCTION_H

#include <cstddef>
#include <string>
#include <vector>

namespace worm::functions {
/*
 * `Function` example:
 * .worm:
 * func announce(s, t)
 *  print(t)
 *  print(t)
 *  sleep(s)
 *
 * .cpp:
 * `Function` "announce" would then internally look like this:
 *  sub_funcs_ == [print, print, sleep]
 *  param_mapping_ == [[1], [1], [0]]
 * --because then-->
 *  -> sub_funcs_[0] (== print) uses announce()'s second param t
 *  -> sub_funcs_[1] (== print) uses announce()'s second param t
 *  -> sub_funcs_[2] (== sleep) uses announce()'s first param s
 */
class Function {
public:
  /*
   * Used to return a hash that can be used as key to store the function
   * unambiguously in a data structure like a hashmap
   */
  static std::string hash_code(const std::string& func_name, std::size_t param_count);

  /*
   * @precondition: Before trying to call the constructor, the calling site's parser
   * should verify that the lines that it passes are the declaration of exactly one
   * function and should only pass the relevant lines
   */
  Function(const std::vector<std::string>& lines);
  Function(
    const std::string& name,
    std::size_t param_count,
    void (*base_func)(const std::vector<std::string>&)
  );

  void operator()(const std::vector<std::string>&) const;

  std::string hash_code() const;

  bool operator==(const Function& other) const;
  bool operator!=(const Function& other) const;

private:
  std::string name;
  std::size_t param_count;
  /*
   * Sub-functions that this function should call sequentially
   * Stored as pointers to improve performance
   */
  std::vector<Function*> sub_funcs;
  /*
   * Mapping to assign the correct parameters provided in `operator()()`
   * to the sub-functions
   *
   * Example:
   * If the first element in `param_mapping_` is [0, 2], it means that the
   * first function in `sub_funcs_` takes the first and third parameters
   * that are provided to `operator()()`
   */
  std::vector<std::vector<std::pair<std::string, std::size_t>>> param_mapping;
  /*
   * Base function, used as entry point to introduce C++ functions to Worm
   * Example use case:
   * to store the behavior of (.worm:) "print("Hello, world!")",
   * you can instantiate following function: Function {wstd::print}
   *
   * It should always be the case that EITHER a base function exists
   * OR sub-functions and optional respective parameter mappings
   * (a base function should basically represent the lowest level function-kind
   * and other functions should either be a base function
   * or build on top of those)
   */
  void (*const base_func)(const std::vector<std::string>&) = nullptr;
};
} // namespace worm::functions

#endif // WORM_FUNCTIONS_FUNCTION_H
