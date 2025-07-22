#ifndef HELPER_H
#define HELPER_H
#include <filesystem>
#include <string>
class Helper {
 public:
  static std::string get_db_path(char* exe);
};
#endif  // HELPER_H