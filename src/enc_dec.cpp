#include "enc_dec.hpp"

#include <crypt.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/gcm.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include <array>
#include <fstream>
#include <iostream>
#include <print>
#include <string>

constexpr size_t ITERATIONS = 100'000;
constexpr CryptoPP::byte PURPOSE = 0;  // No purpose

std::pair<Key, Salt> EncDec::generate_key_salt(
    const std::string &password) noexcept {
  std::pair<Key, Salt> result;

  // Secure random salt generation
  CryptoPP::AutoSeededRandomPool prng;
  prng.GenerateBlock(result.second.data(), result.second.size());

  // Key derivation using PBKDF2-HMAC-SHA256
  const CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
  pbkdf.DeriveKey(result.first.data(), result.first.size(), PURPOSE,
                  reinterpret_cast<const CryptoPP::byte *>(password.data()),
                  password.size(), result.second.data(), result.second.size(),
                  ITERATIONS,  // Iteration count
                  0);          // Default flags

  return result;
}

Key EncDec::generate_key_from_salt(const std::string &password,
                                   const Salt &salt) noexcept {
  Key key;
  const CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
  pbkdf.DeriveKey(key.data(), key.size(), PURPOSE,
                  reinterpret_cast<const CryptoPP::byte *>(password.data()),
                  password.size(), salt.data(), salt.size(), ITERATIONS, 0);
  return key;
}
Nonce EncDec::generate_nonce() noexcept {
  Nonce nonce;
  CryptoPP::AutoSeededRandomPool prng;

  prng.GenerateBlock(nonce.data(), nonce.size());
  return nonce;
}

std::string EncDec::encrypt(std::string &plain_text,
                            const std::string &password) {
  auto [key, salt] = generate_key_salt(password);
  Nonce nonce = generate_nonce();

  if (plain_text.empty()) {
    throw std::invalid_argument("Empty plain text to encrypt");
  }

  try {
    std::string cipher;
    CryptoPP::GCM<CryptoPP::AES>::Encryption gcm_aes;
    gcm_aes.SetKeyWithIV(key.data(), key.size(), nonce.data(), nonce.size());

    CryptoPP::AuthenticatedEncryptionFilter encryption_filter(
        gcm_aes, new CryptoPP::StringSink(cipher),
        false,  // MAC at end
        16      // MAC length
    );

    encryption_filter.Put(
        reinterpret_cast<const CryptoPP::byte *>(plain_text.data()),
        plain_text.size());
    encryption_filter.MessageEnd();

    // Clear plaintext memory
    std::fill(plain_text.begin(), plain_text.end(), '\0');
    plain_text.clear();

    // Build final output: salt || nonce || cipher+MAC
    std::string result;
    result.reserve(salt.size() + nonce.size() + cipher.size());
    result.assign(reinterpret_cast<const char *>(salt.data()), salt.size());
    result.append(reinterpret_cast<const char *>(nonce.data()), nonce.size());
    result.append(cipher);

    return result;
  } catch (const CryptoPP::Exception &e) {
    std::cerr << "CryptoPP error: " << e.what() << std::endl;
    throw std::runtime_error("Encryption failed");
  } catch (...) {
    std::cerr << "Unknown error occurred during encryption" << std::endl;
    throw std::runtime_error("Unknown encryption error");
  }
}

std::string EncDec::decrypt(const std::string &cipher,
                            const std::string &password) {
  if (cipher.size() <= 16 + 12 + 16) {  // salt + nonce + MAC at minimum
    throw std::runtime_error(
        "File too small to contain [salt nonce ciphertext MAC]");
  }

  std::string recovered;
  try {
    // Extract salt (first 16 bytes)
    Salt salt;
    memcpy(salt.data(), cipher.data(), salt.size());

    // Regenerate key
    Key key = generate_key_from_salt(password, salt);

    // Extract nonce (next 12 bytes)
    Nonce nonce;
    memcpy(nonce.data(), cipher.data() + salt.size(), nonce.size());

    // Extract ciphertext and MAC
    size_t offset = salt.size() + nonce.size();
    std::string enc = cipher.substr(offset, cipher.size() - offset - 16);
    std::string mac = cipher.substr(cipher.size() - 16);

    // Setup AES-GCM for decryption
    CryptoPP::GCM<CryptoPP::AES>::Decryption aes_gcm;
    aes_gcm.SetKeyWithIV(key.data(), key.size(), nonce.data(), nonce.size());

    CryptoPP::AuthenticatedDecryptionFilter decryption_filter(
        aes_gcm, new CryptoPP::StringSink(recovered),
        CryptoPP::AuthenticatedDecryptionFilter::MAC_AT_BEGIN |
            CryptoPP::AuthenticatedDecryptionFilter::THROW_EXCEPTION,
        16);

    // Feed MAC first, then ciphertext
    decryption_filter.Put((const CryptoPP::byte *)mac.data(), mac.size());
    decryption_filter.Put((const CryptoPP::byte *)enc.data(), enc.size());
    decryption_filter.MessageEnd();

  } catch (const CryptoPP::Exception &e) {
    std::cerr << "CryptoPP error: " << e.what() << std::endl;
    throw std::runtime_error("Decryption failed");
  } catch (...) {
    std::cerr << "Unexpected error during decryption" << std::endl;
    throw std::runtime_error("Unknown decryption error");
  }

  return recovered;
}
