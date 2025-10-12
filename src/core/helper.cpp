#include "core/helper.hpp"

#include <filesystem>
#include <optional>
#include <string>
std::optional<size_t> helper::parse_size_t(const std::string& str) {
  if (str.empty() || str[0] == '-') return std::nullopt;
  try {
    size_t pos;
    size_t val = std::stoul(str, &pos);
    if (pos != str.length())
      return std::nullopt;
    else
      return val;
  } catch (...) {
    return std::nullopt;
  }
}