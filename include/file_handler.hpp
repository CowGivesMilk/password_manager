#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

class Entry {
 public:
  std::string username, password, site;
  Entry(const std::string username, const std::string password,
        const std::string site);
};
class FileHandler {
 private:
  std::string file_path;

 public:
  FileHandler(const std::string file_path) : file_path(file_path) {}
  bool add_entry(const Entry& entry);
};

#endif  // FILE_HANDLER_H