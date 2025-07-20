#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

class Entry {
 public:
  std::string username, password, site;
  Entry(const std::string username, const std::string password,
        const std::string site);
  const std::string to_string() const;
};
class FileHandler {
 private:
  std::string file_path;

 public:
  FileHandler(const std::string file_path);
  bool add_entry(const Entry& entry);
  std::string get_file_path() const;
};

#endif  // FILE_HANDLER_H