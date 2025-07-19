#include "file_handler.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

Entry::Entry(const std::string username, const std::string password,
             const std::string site)
    : username(username), password(password), site(site) {}

bool FileHandler::add_entry(const Entry& entry) {
  std::ofstream out_file(this->file_path, std::ios::app);
  if (!out_file) {
    std::cerr << "Error Cannot open file\n";
    return false;
  }
  out_file << entry.site << ' ' << entry.username << ' ' << entry.password
           << std::endl;

  return true;
}