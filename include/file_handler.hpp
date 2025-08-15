#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <cryptopp/aes.h>

#include <filesystem>
#include <string>
#include <vector>
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
  static std::string read_file(
      const std::filesystem::path path);  // Reads full file
  static std::string read_file(const std::string path);
  static std::array<CryptoPP::byte, 12> read_nonce(
      const std::filesystem::path path);
  static std::array<CryptoPP::byte, 12> read_nonce(const std::string file_path);
  static std::array<CryptoPP::byte, 16> read_tag(
      const std::filesystem::path path);
  static std::array<CryptoPP::byte, 16> read_tag(const std::string file_path);
  static std::string read_file_data(const std::filesystem::path path);
  static std::string read_file_data(const std::string file_path);
};

#endif  // FILE_HANDLER_H