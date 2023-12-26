//
// Created by 13199 on 2023/12/25.
//
#include <string>

// check: Num Letters underline
bool checkNLU(std::string &str) {
  for (char c : str) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
      continue;
    }
    else return false;
  }
  return true;
};
bool checkVA(const std::string& str) {
  for (char c : str) {
    if (c < 32 || c > 126) {
      return false;
    }
  }
  return true;
}
bool checkVAN(const std::string& str) {
  for (char c : str) {
    if ((c < 32 || c > 126) || c == '"') {
      return false;
    }
  }
  return true;
}