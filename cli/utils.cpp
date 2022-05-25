#include "./include/utils.hpp"

namespace utils {

std::regex regex_literal_hex =
    std::regex("0[xX]([0-9A-Fa-f]+)|[hH]'([0-9A-Fa-f]+)'|([0-9A-Fa-f]+)");

std::regex regex_literal_bin = std::regex("[bB]'([01]+)'");
std::regex regex_literal_dec = std::regex("[dD]'([0-9]+)'");

std::string get_first_not_empty_submatch(std::smatch const &sm) {
  for (uint8_t i = 1; i < sm.size(); i++) {
    if (sm[i].length() > 0) {
      return sm[i];
    }
  }
  return "";
}

int to_literal(std::string const &literal_str) {
  int literal = 0;
  std::smatch sm;
  if (std::regex_match(literal_str, sm, utils::regex_literal_hex)) {
    std::stringstream ss;
    ss << std::hex << utils::get_first_not_empty_submatch(sm);
    ss >> literal;
  } else if (std::regex_match(literal_str, sm, utils::regex_literal_dec)) {
    std::stringstream ss;
    ss << utils::get_first_not_empty_submatch(sm);
    ss >> literal;
  } else if (std::regex_match(literal_str, sm, utils::regex_literal_bin)) {
    literal = std::stoi(utils::get_first_not_empty_submatch(sm), nullptr, 2);
  }
  return literal;
}

} // namespace utils
