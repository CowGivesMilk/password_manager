#include "enc_dec.hpp"

#include <catch2/catch_all.hpp>

std::pair<std::array<CryptoPP::byte, 32>, std::array<CryptoPP::byte, 16>>
    key_salt = EncDec::generate_key_salt("Password");
std::array<CryptoPP::byte, 32> key =
    EncDec::generate_key_from_salt("Password", key_salt.second);
TEST_CASE(
    "Generated key from password and Random salt must be equal to Generated "
    "key from password and same salt") {
  REQUIRE(key_salt.first == key);
}
TEST_CASE("Encryptor decryptor") {
  std::string orignal_text = "hjedlikwqherdl;ofihasdlf";
  std::string orignal_text_copy = orignal_text;
  auto key_salt = EncDec::generate_key_salt("This is very strong password");
  auto nonce = EncDec::generate_nonce();

  std::string cipher = EncDec::encrypt(orignal_text, key_salt.first, nonce);
  std::string decrypted_text = EncDec::decrypt(cipher, key_salt.first);

  REQUIRE(orignal_text_copy == decrypted_text);
}