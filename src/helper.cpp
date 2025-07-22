#include "helper.hpp"

#include <filesystem>
#include <string>
std::string helper::get_db_path(char* exe) {
  std::filesystem::path path = std::filesystem::absolute(exe).parent_path();
  return path.string() + "/data/password.csv";
}
bool helper::parse_size_t(const std::string& str, size_t& out) {
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