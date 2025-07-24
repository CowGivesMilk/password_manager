#ifndef ENC_DEC_H
#define ENC_DEC_H
#include <cryptopp/pwdbased.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>
struct DerivedKey {
  std::array<CryptoPP::byte, 32> key;
  std::array<CryptoPP::byte, 16> salt;
};
class EncDec {
 public:
  static DerivedKey generate_key_salt(const std::string& password);
  static std::array<CryptoPP::byte, 32> generate_key_from_salt(
      const std::string& password, const std::array<CryptoPP::byte, 16>& salt);
};
#endif  // ENC_DEC_H