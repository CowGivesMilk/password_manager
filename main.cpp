#include <filesystem>
#include <print>

#include "file_handler.hpp"
#include "password_generator.hpp"
int main(int argc, char** argv) {
  PasswordGenerator pg;
  std::println("Random password of length 4 is {}", pg.generatePassword(4ul));
  std::string path =
      std::filesystem::absolute(argv[argc - argc]).parent_path().string() +
      "/buffer_file.txt";
  FileHandler fh(path);
  fh.write("This text should appear");
  return 0;
}
