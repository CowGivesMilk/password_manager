#include "file_handler.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

Entry::Entry(const std::string username, const std::string password,
             const std::string site)
    : username(username), password(password), site(site) {}

const std::string Entry::to_string() const {
  return std::format("{} {} {}", this->site, this->username, this->password);
}

FileHandler::FileHandler(const std::string file_path) : file_path(file_path) {}

bool FileHandler::add_entry(const Entry& entry) {
  std::ofstream out_file(this->get_file_path(), std::ios::app);
  if (!out_file) {
    std::cerr << "Error Cannot open file\n";
    return false;
  }
  if (std::filesystem::file_size(this->get_file_path()) ==
      0) {  // File is empty. Adding first entry so no newline character
    out_file << entry.to_string();
  } else {
    out_file << std::endl << entry.to_string();
  }

  return out_file.good();
}

std::string FileHandler::get_file_path() const { return file_path; }