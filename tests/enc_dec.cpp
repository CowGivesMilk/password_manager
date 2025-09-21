#include "enc_dec.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Encryptor decryptor") {
  std::string orignal_text = "Orignal text";
  std::string orignal_text_copy = orignal_text;
  std::string password = "passwd";

  std::string cipher = EncDec::encrypt(orignal_text, password);
  std::string decrypted_text = EncDec::decrypt(cipher, password);

  REQUIRE(decrypted_text == orignal_text_copy);
}