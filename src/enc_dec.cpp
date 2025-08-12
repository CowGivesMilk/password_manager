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
#include <string>
const size_t ITERATIONS = 100000;
const CryptoPP::byte PURPOSE = 0; // No purpose

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
                  ITERATIONS, // Iteration count
                  0);         // Default flags

  return result;
}

std::array<CryptoPP::byte, 32>
EncDec::generate_key_from_salt(const std::string &password,
                               const std::array<CryptoPP::byte, 16> &salt) {
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
void EncDec::encrypt(const std::string &file_path,
                     const std::array<CryptoPP::byte, 32> &key,
                     const std::array<CryptoPP::byte, 16> &salt,
                     const std::array<CryptoPP::byte, 12> &nonce) {
  std::ifstream in(file_path, std::ios::binary);
  if (!in) {
    throw std::runtime_error("Error opening file");
  }
  in.seekg(0, std::ios::end);
  size_t size = in.tellg();
  in.seekg(0, std::ios::beg);
  std::string plain_text(size, '\0');
  in.read(&plain_text[0], size);
  in.close();

  std::string cipher_text;
  std::array<CryptoPP::byte, 16> TAG{0};
  try {
    CryptoPP::GCM<CryptoPP::AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(key.data(), key.size(), nonce.data(), nonce.size());

    CryptoPP::AuthenticatedEncryptionFilter ef(
        encryptor, new CryptoPP::StringSink(cipher_text), false, TAG.size());

    ef.ChannelPut("AAD", salt.data(), salt.size());
    ef.ChannelMessageEnd("AAD");

    ef.ChannelPut("",
                  reinterpret_cast<const CryptoPP::byte *>(plain_text.data()),
                  plain_text.size());

    ef.ChannelMessageEnd("");
  } catch (const CryptoPP::Exception &e) {
    throw std::runtime_error("Encryption Error: " + std::string(e.what()));
  }
  std::string out_path = file_path + ".enc";
  std::ofstream out(out_path, std::ios::binary);
  if (!out) {
    throw std::runtime_error("Error creating output file");
  }
  out.write(reinterpret_cast<const char *>(salt.data()), salt.size());
  out.write(cipher_text.data(), cipher_text.size());
  out.write(reinterpret_cast<const char *>(TAG.data()), TAG.size());
  out.close();
}

void EncDec::decrypt(const std::string &encrypted_file_path,
                     const std::array<CryptoPP::byte, 32> &key,
                     const std::array<CryptoPP::byte, 12> &nonce) {
  // Read encrypted file
  std::ifstream in(encrypted_file_path, std::ios::binary);
  if (!in) {
    throw std::runtime_error("Error opening encrypted file");
  }

  in.seekg(0, std::ios::end);
  size_t size = in.tellg();
  in.seekg(0, std::ios::beg);

  // Verify minimum file size (salt + tag)
  const size_t MIN_SIZE = 16 + 16; // salt + tag
  if (size < MIN_SIZE) {
    throw std::runtime_error("Invalid encrypted file format");
  }

  // Read file components
  std::array<CryptoPP::byte, 16> salt;
  in.read(reinterpret_cast<char *>(salt.data()), salt.size());

  const size_t ciphertext_size = size - salt.size() - 16;
  std::string ciphertext(ciphertext_size, '\0');
  in.read(&ciphertext[0], ciphertext_size);

  std::array<CryptoPP::byte, 16> tag;
  in.read(reinterpret_cast<char *>(tag.data()), tag.size());
  in.close();

  // Setup decryption
  std::string plaintext;
  try {
    CryptoPP::GCM<CryptoPP::AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(key.data(), key.size(), nonce.data(), nonce.size());

    // Configure for verification (THROW_EXCEPTION ensures failed verification
    // throws)
    CryptoPP::AuthenticatedDecryptionFilter df(
        decryptor, new CryptoPP::StringSink(plaintext),
        CryptoPP::AuthenticatedDecryptionFilter::MAC_AT_BEGIN |
            CryptoPP::AuthenticatedDecryptionFilter::THROW_EXCEPTION,
        16 // tag size
    );

    // Critical: Process components in EXACT order:
    // 1. Provide tag first (MAC_AT_BEGIN)
    df.ChannelPut("", tag.data(), tag.size());

    // 2. Provide AAD (salt)
    df.ChannelPut("AAD", salt.data(), salt.size());
    df.ChannelMessageEnd("AAD");

    // 3. Provide ciphertext
    df.ChannelPut("",
                  reinterpret_cast<const CryptoPP::byte *>(ciphertext.data()),
                  ciphertext.size());
    df.ChannelMessageEnd("");

    // If we reach here, verification succeeded
  } catch (const CryptoPP::HashVerificationFilter::HashVerificationFailed &) {
    throw std::runtime_error(
        "Decryption failed: Authentication tag verification failed");
  } catch (const CryptoPP::Exception &e) {
    throw std::runtime_error("Decryption error: " + std::string(e.what()));
  }

  // Write decrypted file
  std::string output_path = encrypted_file_path;
  if (output_path.size() > 4 &&
      output_path.substr(output_path.size() - 4) == ".enc") {
    output_path.erase(output_path.size() - 4);
  } else {
    output_path += ".decrypted";
  }

  std::ofstream out(output_path, std::ios::binary);
  if (!out) {
    throw std::runtime_error("Error creating output file");
  }
  out.write(plaintext.data(), plaintext.size());
  out.close();
}
