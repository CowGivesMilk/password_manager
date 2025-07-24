#include "enc_dec.hpp"

#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>  // For PKCS5_PBKDF2_HMAC
#include <cryptopp/sha.h>

#include <array>
// #include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

DerivedKey EncDec::generate_key_salt(const std::string& password) {
  DerivedKey result;
  const CryptoPP::byte PURPOSE = 0;  // No purpose

  // Secure random salt generation
  CryptoPP::AutoSeededRandomPool prng;
  prng.GenerateBlock(result.salt.data(), result.salt.size());

  // Key derivation using PBKDF2-HMAC-SHA256
  CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
  pbkdf.DeriveKey(result.key.data(), result.key.size(), PURPOSE,
                  reinterpret_cast<const CryptoPP::byte*>(password.data()),
                  password.size(), result.salt.data(), result.salt.size(),
                  100'000,  // Iteration count
                  0);       // Default flags

  return result;
}

std::array<CryptoPP::byte, 32> EncDec::generate_key_from_salt(
    const std::string& password, const std::array<CryptoPP::byte, 16>& salt) {
  std::array<CryptoPP::byte, 32> key;
  const CryptoPP::byte PURPOSE = 0;  // No purpose
  CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
  pbkdf.DeriveKey(key.data(), key.size(), PURPOSE,
                  reinterpret_cast<const CryptoPP::byte*>(password.data()),
                  password.size(), salt.data(), salt.size(), 100'000, 0);
  return key;
}
