#include "file_handler.hpp"

#include <filesystem>
#include <fstream>
#include <string>

FileHandler::FileHandler(const std::string& file_path) : file_path(file_path) {}

std::string FileHandler::get_file_path() const { return this->file_path; }

std::string FileHandler::read() {
  // Open the stream to 'lock' the file.
  const std::filesystem::path path{this->get_file_path()};
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = std::filesystem::file_size(path);

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  return result;
}

bool FileHandler::write(const std::string& str) const {
  std::ofstream file(this->get_file_path());
  if (!file) {
    throw std::runtime_error("Cannot open file in output mode");
  }
  file << str;
  return file.good();
}
