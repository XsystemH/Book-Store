//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_ACCOUNT_H
#define CODE_ACCOUNT_H

#include <cstring>
#include "book.h"

class Account_node {
public:

  char maxn[32]; // UserID
  char minn[32];
  int pos;
  int next;

public:

  Account_node() {
    strcpy(maxn, "");
    strcpy(minn, "");
    pos = 0;
    next = -1;
  }
  ~Account_node() = default;
};

class Account_info {
public:

  char index[32]; // UserID
  char value[32]; // UserName
  char Password[32];

  Account_info& operator=(const Account_info &b) {
    if (&b == this) return *this;
    strcpy(index, b.index);
    strcpy(value, b.value);
    strcpy(Password, b.Password);
    return *this;
  }
};



#endif //CODE_ACCOUNT_H
