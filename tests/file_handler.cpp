#define CATCH_CONFIG_RUNNER
#include "file_handler.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/catch_session.hpp>
#include <filesystem>
using namespace Catch::Matchers;
char* argv0;
int main(int argc, char* argv[]) {
  argv0 = argv[0];
  return Catch::Session().run(argc, argv);
}
TEST_CASE("File Handler") {
  std::string text = "some text";
  std::string path = std::filesystem::absolute(argv0).parent_path().string() +
                     "/test_text.txt";
  FileHandler fh(path);
  fh.write(text);
  REQUIRE(text == fh.read());
}
