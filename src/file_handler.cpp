#include "file_handler.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <limits>
#include <print>
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
    std::println("Error Cannot open file");
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
std::string FileHandler::get_file_path() const { return this->file_path; }
std::string FileHandler::read_file(const std::filesystem::path path) {
  // Open the stream to 'lock' the file.
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = std::filesystem::file_size(path);

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  return result;
}
std::string FileHandler::read_file(const std::string path) {
  return FileHandler::read_file(std::filesystem::path(path));
}
std::array<CryptoPP::byte, 12> FileHandler::read_nonce(
    const std::filesystem::path path) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }
  const size_t FILE_SIZE = std::filesystem::file_size(path);
  if (FILE_SIZE < 12 + 16) {
    throw std::runtime_error(
        "File is invalid File does not contain IV and/or TAG " + path.string());
  }
  std::array<CryptoPP::byte, 12> nonce;
  file.seekg(0);
  if (!file) {
    throw std::runtime_error("Seek to start position failed");
  }
  file.read((char*)nonce.data(), 12);
  return nonce;
}
std::array<CryptoPP::byte, 12> FileHandler::read_nonce(
    const std::string file_path) {
  std::filesystem::path path(file_path);
  return FileHandler::read_nonce(path);
}
std::array<CryptoPP::byte, 16> FileHandler::read_tag(
    const std::filesystem::path path) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }
  const size_t FILE_SIZE = std::filesystem::file_size(path);
  if (FILE_SIZE < 12 + 16) {
    throw std::runtime_error(
        "File is invalid File does not contain IV and/or TAG " + path.string());
  }
  std::array<CryptoPP::byte, 16> nonce;
  file.seekg(FILE_SIZE - 16);
  if (!file) {
    throw std::runtime_error("Seek to start position failed");
  }
  file.read((char*)nonce.data(), 16);
  return nonce;
}
std::array<CryptoPP::byte, 16> FileHandler::read_tag(
    const std::string file_path) {
  std::filesystem::path path(file_path);
  return FileHandler::read_tag(path);
}
std::string FileHandler::read_file_data(const std::filesystem::path path) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }
  const size_t FILE_SIZE = std::filesystem::file_size(path);
  if (FILE_SIZE < 12 + 16) {
    throw std::runtime_error(
        "File is invalid File does not contain IV and/or TAG " + path.string());
  }

  std::string edata(FILE_SIZE - 12 - 16, '\0');
  file.seekg(12);
  if (!file.is_open()) {
    std::runtime_error("Can not seek to start of data after IV: " +
                       path.string());
  }
  file.read(edata.data(), edata.size());
  return edata;
}
std::string FileHandler::read_file_data(const std::string file_path) {
  return FileHandler::read_file_data(std::filesystem::path(file_path));
}