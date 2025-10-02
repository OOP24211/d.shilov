#pragma once
#include <iostream>
#include <string>

class Error : public std::exception {
 private:
  std::string mes_;

 public:
  explicit Error(const std::string &err);
  const char* what() const noexcept override;
};

