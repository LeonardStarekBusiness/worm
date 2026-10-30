#ifndef WORM_UTIL_STRING_H
#define WORM_UTIL_STRING_H

#include <string>
#include <string_view>
#include <vector>

namespace util::string {
void trim(std::string&);
std::vector<std::string> parse_params(std::string_view);
}

#endif // WORM_UTIL_STRING_H
