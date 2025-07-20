#include <filesystem>
#include <string>
std::string get_db_path(char* exe) {
  std::filesystem::path path = std::filesystem::absolute(exe).parent_path();
  return path.string() + "/data/password.csv";
}