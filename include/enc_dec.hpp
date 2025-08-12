#ifndef ENC_DEC_H
#define ENC_DEC_H
#include <cryptopp/pwdbased.h>

#include <array>
#include <string>
class EncDec {
public:
  static std::pair<std::array<CryptoPP::byte, 32>,
                   std::array<CryptoPP::byte, 16>>
  generate_key_salt(const std::string &password);

  static std::array<CryptoPP::byte, 32>
  generate_key_from_salt(const std::string &password,
                         const std::array<CryptoPP::byte, 16> &salt);
  static std::array<CryptoPP::byte, 12> generate_nonce();

  static void encrypt(const std::string &file_path,
                      const std::array<CryptoPP::byte, 32> &key,
                      const std::array<CryptoPP::byte, 16> &salt,
                      const std::array<CryptoPP::byte, 12> &nonce);
  static void decrypt(const std::string &file_path,
                      const std::array<CryptoPP::byte, 32> &key,
                      const std::array<CryptoPP::byte, 12> &nonce);
};
#endif // ENC_DEC_H
