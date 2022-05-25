#ifndef H_INCLUDE_CLI_UTILS
#define H_INCLUDE_CLI_UTILS

#include <iostream>
#include <regex>

namespace utils {

extern std::regex regex_literal_hex;
extern std::regex regex_literal_bin;
extern std::regex regex_literal_dec;

std::string get_first_not_empty_submatch(std::smatch const &sm);
int to_literal(std::string const &literal_str);

} // namespace utils
#endif
