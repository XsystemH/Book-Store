//
// Created by 13199 on 2023/12/25.
//
#include <string>
#include <error.h>

// check: Num Letters underline
bool checkNLU(std::string &str, int size) {
  if (str.size() > size) {
    return false;
  }
  for (char c : str) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
      continue;
    }
    else return false;
  }
  return true;
};
bool checkVA(const std::string& str, int size) {
  if (str.size() > size) {
    return false;
  }
  for (char c : str) {
    if (c < 32 || c > 126) {
      return false;
    }
  }
  return true;
}
bool checkVAN(const std::string& str, int size) {
  if (str.size() > size) {
    return false;
  }
  for (char c : str) {
    if ((c < 32 || c > 126) || c == '"') {
      return false;
    }
  }
  return true;
}
double StringtoDouble (std::string &str) {
  if (str.size() > 13 || *str.begin() == '.' || str.back() == '.') {
    throw error();
  }
  bool num = false;
  bool dot = false;
  double ans = 0.000;
  int a = 0;
  double b = 0.00;
  double t = 0.100;
  for (char c : str) {
    if ((c < '0' || c > '9') && c != '.') throw error();
    if (c == '.') {
      if (dot || !num) throw error(); // more than one dot
      else dot = true;
    }
    else {
      num = true;
      if (dot) {
        b *= 10;
        b += int (c - '0') * t;
        t /= 10;
      }
      else {
        a *= 10;
        a += int (c - '0');
      }
    }
  }
  ans = a + b;
  return ans;
}