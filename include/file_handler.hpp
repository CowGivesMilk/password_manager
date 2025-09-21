#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

class FileHandler {
 private:
  std::string file_path;

 public:
  FileHandler(const std::string file_path);
  [[nodiscard]] std::string get_file_path() const;
  [[nodiscard]] std::string read();
  bool write(const std::string& str) const;
};

#endif  // FILE_HANDLER_H
