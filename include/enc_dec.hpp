#ifndef ENC_DEC_H
#define ENC_DEC_H
#include <cryptopp/aes.h>

#include <array>
#include <optional>
#include <string>
class EncDec {
 public:
  static std::pair<std::array<CryptoPP::byte, 32>,
                   std::array<CryptoPP::byte, 16>>
  generate_key_salt(const std::string &password) noexcept;

  static std::array<CryptoPP::byte, 32> generate_key_from_salt(
      const std::string &password,
      const std::array<CryptoPP::byte, 16> &salt) noexcept;
  static std::array<CryptoPP::byte, 12> generate_nonce() noexcept;

  static std::string encrypt(const std::string &plain_text,
                             const std::array<CryptoPP::byte, 32> &key,
                             const std::array<CryptoPP::byte, 12> &nonce);
  static std::string decrypt(const std::string &cipher,
                             const std::array<CryptoPP::byte, 32> &key);
};
#endif  // ENC_DEC_H
