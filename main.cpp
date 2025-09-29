#include <print>

#include "data_handler.hpp"
using namespace std::chrono_literals;
int main(int, char**) {
  json j_arr = json::array();
  Entry e1("Title1", "uname1", "passwd1", "site1", "note1",
           std::vector<std::string>{"tag1.1", "tag1.2"}, std::nullopt);
  Entry e2("Title2", "uname2", "passwd2", "site2", "note2",
           std::vector<std::string>{"tag2.1", "tag2.2"}, std::nullopt);
  Entry e3("Title3", "uname3", "passwd3", "site3", "note3",
           std::vector<std::string>{"tag3.1", "tag3.2"},
           std::chrono::system_clock::now() + 4000000s);
  json cj = combine_json(std::vector<Entry>{e1, e2, e3});
  std::println("{}", cj.dump(2));
  return 0;
}