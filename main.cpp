#include <chrono>
#include <iostream>
#include <optional>
#include <print>
#include <string>

#include "enc_dec.hpp"
#include "file_handler.hpp"
#include "password_generator.hpp"
using namespace std::chrono_literals;
int main(int argc, char **argv) {
  std::string password;
  if (argc == 1) {
    std::print("Enter the passowrd: ");
    std::getline(std::cin, password);
  } else {
    password = argv[1];
  }
  std::println("Password: {}", password);
  std::pair<std::array<CryptoPP::byte, 32>, std::array<CryptoPP::byte, 16>>
      key_salt = EncDec::generate_key_salt(password);
  std::array<CryptoPP::byte, 32> key_from_salt =
      EncDec::generate_key_from_salt(password, key_salt.second);
  std::array<CryptoPP::byte, 12> nonce = EncDec::generate_nonce();

  std::println("First Key");
  for (auto c : key_salt.first) {
    std::print("{:X} ", static_cast<uint8_t>(c));
  }
  std::println();

  std::println("Salt");
  for (auto c : key_salt.second) {
    std::print("{:X} ", static_cast<uint8_t>(c));
  }
  std::println();

  std::println("Key from salt");
  for (auto c : key_from_salt) {
    std::print("{:X} ", static_cast<uint8_t>(c));
  }
  std::println();

  std::println("nonce");
  for (auto c : nonce) {
    std::print("{:X} ", static_cast<uint8_t>(c));
  }
  std::println();
  const std::string orignal_file_path =
      "/home/nimes/Documents/projects/password_manager/poem.txt";
  EncDec::encrypt(orignal_file_path, key_salt.first, nonce);
  EncDec::decrypt(orignal_file_path + ".enc", key_salt.first);
  std::println();
  PasswordGenerator pg(true, true, true, true);
  std::println("Random Characters: {}", pg.generatePassword(8));
  Entry e("My Title", "user123", std::nullopt, "example.com", std::nullopt,
          std::nullopt);
  std::println("{}", e.to_string());
  return 0;
}
