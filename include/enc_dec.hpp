#ifndef ENC_DEC_H
#define ENC_DEC_H
#include <cryptopp/aes.h>

#include <array>
#include <string>
typedef std::array<CryptoPP::byte, 32> Key;
typedef std::array<CryptoPP::byte, 16> Salt;
typedef std::array<CryptoPP::byte, 12> Nonce;
class EncDec {
 private:
  [[nodiscard]] static std::pair<Key, Salt> generate_key_salt(
      const std::string &password) noexcept;

  [[nodiscard]] static Key generate_key_from_salt(const std::string &password,
                                                  const Salt &salt) noexcept;
  [[nodiscard]] static Nonce generate_nonce() noexcept;

 public:
  [[nodiscard]] static std::string encrypt(std::string &plain_text,
                                           const std::string &password);
  [[nodiscard]] static std::string decrypt(const std::string &cipher,
                                           const std::string &password);
};
#endif  // ENC_DEC_H