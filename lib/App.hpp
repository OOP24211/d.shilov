#pragma once
#include <string>
#include <vector>
#include "../lib/ReadFile.hpp"
#include "../lib/WordCounter.hpp"
#include "../lib/RecordFile.hpp"
#include "../lib/Error.hpp"

class App {
 public:
  App(int argc, char* argv[]);

  App(std::size_t argc, std::vector<std::string, std::allocator<std::string>> argv);
  int run();

 private:
  std::string argv1_;
  std::string argv2_;
  int argc_;
};
