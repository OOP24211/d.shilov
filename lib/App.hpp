#pragma once
#include <string>
#include "../lib/ReadFile.hpp"
#include "../lib/WordCounter.hpp"
#include "../lib/RecordFile.hpp"
#include "../lib/Error.hpp"

class App {
 public:
  explicit App(int argc, char* argv[]);
  int run();

 private:
  std::string argv1_;
  std::string argv2_;
  int argc_;
};
