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