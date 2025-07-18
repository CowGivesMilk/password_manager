#include "input_validator.hpp"

#include <string>

bool parse_size_t(const std::string& str, size_t& out) {
  try {
    size_t pos;
    unsigned long val = std::stoul(str, &pos);
    if (pos != str.length()) return false;  // extra characters
    out = val;
    return true;
  } catch (...) {
    return false;
  }
}