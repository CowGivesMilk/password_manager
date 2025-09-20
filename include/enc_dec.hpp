#ifndef ENC_DEC_H
#define ENC_DEC_H
#include <cryptopp/aes.h>

#include <array>
#include <optional>
#include <string>
typedef std::array<CryptoPP::byte, 32> Key;
typedef std::array<CryptoPP::byte, 16> Salt;
typedef std::array<CryptoPP::byte, 12> Nonce;
class EncDec {
 public:
  static std::pair<Key, Salt> generate_key_salt(
      const std::string &password) noexcept;

  static Key generate_key_from_salt(const std::string &password,
                                    const Salt &salt) noexcept;
  static Nonce generate_nonce() noexcept;

  static std::string encrypt(std::string &plain_text, const Key &key,
                             const Nonce &nonce);
  static std::string decrypt(const std::string &cipher, const Key &key);
};
#endif  // ENC_DEC_H