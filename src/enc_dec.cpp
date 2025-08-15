#include "enc_dec.hpp"

#include <crypt.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/gcm.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include <array>
#include <fstream>
#include <iostream>
#include <print>
#include <string>

#include "file_handler.hpp"
const size_t ITERATIONS = 100000;
const CryptoPP::byte PURPOSE = 0;  // No purpose

std::pair<std::array<CryptoPP::byte, 32>, std::array<CryptoPP::byte, 16>>
EncDec::generate_key_salt(const std::string &password) {
  std::pair<std::array<CryptoPP::byte, 32>, std::array<CryptoPP::byte, 16>>
      result;

  // Secure random salt generation
  CryptoPP::AutoSeededRandomPool prng;
  prng.GenerateBlock(result.second.data(), result.second.size());

  // Key derivation using PBKDF2-HMAC-SHA256
  CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
  pbkdf.DeriveKey(result.first.data(), result.first.size(), PURPOSE,
                  reinterpret_cast<const CryptoPP::byte *>(password.data()),
                  password.size(), result.second.data(), result.second.size(),
                  ITERATIONS,  // Iteration count
                  0);          // Default flags

  return result;
}

std::array<CryptoPP::byte, 32> EncDec::generate_key_from_salt(
    const std::string &password, const std::array<CryptoPP::byte, 16> &salt) {
  std::array<CryptoPP::byte, 32> key;
  CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
  pbkdf.DeriveKey(key.data(), key.size(), PURPOSE,
                  reinterpret_cast<const CryptoPP::byte *>(password.data()),
                  password.size(), salt.data(), salt.size(), ITERATIONS, 0);
  return key;
}
std::array<CryptoPP::byte, 12> EncDec::generate_nonce() {
  std::array<CryptoPP::byte, 12> nonce;
  CryptoPP::AutoSeededRandomPool prng;

  prng.GenerateBlock(nonce.data(), nonce.size());
  return nonce;
}
bool EncDec::encrypt(const std::string &file_path,
                     const std::array<CryptoPP::byte, 32> &key,
                     const std::array<CryptoPP::byte, 12> &nonce) {
  std::string plain_text = FileHandler::read_file(file_path);
  if (plain_text.empty()) {
    std::cerr << "Empty file entered for input\n";
    return false;
  }
  std::string cipher;
  CryptoPP::GCM<CryptoPP::AES>::Encryption gcm_aes;
  gcm_aes.SetKeyWithIV(key.data(), key.size(), nonce.data(), nonce.size());

  CryptoPP::AuthenticatedEncryptionFilter encryption_filter(
      gcm_aes, new CryptoPP::StringSink(cipher), false, 16);
  encryption_filter.Put(
      reinterpret_cast<const CryptoPP::byte *>(plain_text.data()),
      plain_text.size());
  encryption_filter.MessageEnd();
  std::ofstream encrypted_file(file_path + ".enc", std::ios::binary);
  encrypted_file.write(reinterpret_cast<const char *>(nonce.data()),
                       nonce.size());
  encrypted_file.write(cipher.data(), cipher.size());
  return encrypted_file.good();
}

bool EncDec::decrypt(const std::string &encrypted_file_path,
                     const std::array<CryptoPP::byte, 32> &key) {
  std::string cipher = FileHandler::read_file(encrypted_file_path);
  if (cipher.size() <= 12 + 16) {  // Check nonce + min ciphertext + MAC
    std::cerr << "Invalid file size\n";
    return false;
  }

  // Extract components
  std::array<CryptoPP::byte, 12> nonce;
  memcpy(nonce.data(), cipher.data(), 12);
  std::string enc = cipher.substr(12, cipher.size() - 12 - 16);
  std::string mac = cipher.substr(cipher.size() - 16);

  // Decrypt
  CryptoPP::GCM<CryptoPP::AES>::Decryption aes_gcm;
  aes_gcm.SetKeyWithIV(key.data(), key.size(), nonce.data(), nonce.size());

  std::string recovered;
  CryptoPP::AuthenticatedDecryptionFilter decryption_filter(
      aes_gcm,
      new CryptoPP::StringSink(recovered),  // Critical: sink for plaintext
      CryptoPP::AuthenticatedDecryptionFilter::MAC_AT_BEGIN |
          CryptoPP::AuthenticatedDecryptionFilter::THROW_EXCEPTION,
      16);

  // Feed MAC first then ciphertext
  decryption_filter.Put((const CryptoPP::byte *)mac.data(), mac.size());
  decryption_filter.Put((const CryptoPP::byte *)enc.data(), enc.size());
  decryption_filter.MessageEnd();

  // std::cout << recovered << std::endl;
  std::ofstream recovered_file(encrypted_file_path + ".dec", std::ios::binary);
  recovered_file.write(recovered.data(), recovered.size());
  return true;
}
