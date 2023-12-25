//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_ACCOUNT_H
#define CODE_ACCOUNT_H

#include <cstring>
#include "blockchain.h"
#include "book.h"
#include "log.h"

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
  int pos; // Privilege

  Account_info& operator=(const Account_info &b) {
    if (&b == this) return *this;
    strcpy(index, b.index);
    strcpy(value, b.value);
    strcpy(Password, b.Password);
    pos = b.pos;
    return *this;
  }
};

extern blockchain<Account_node, Account_info> accounts;

class User {
public:

  int Privilege; // 0-visitor 1-customs 3-staff 7-Boss
// only used in RAM, so it's more convenient to use string
  std::string UserID;
  std::string UserName;
  std::string Password;

  std::string selected; // the ISBN of the selected book
  Book_Information theBook;
  bool real = false; // whether the book's information is completed

public:

  User() = default;
  explicit User(Account_info &b) {
    Privilege = b.pos;
    UserID = b.index;
    UserName = b.value;
    Password = b.Password;
  }

  bool Check(int need); // Check if it meets the permission requirements.
  void su(std::string &UID, std::string &PW);
  void su(std::string &UID); // specialized for Upper User
  void lo(std::string &UID); // log out
  void rg(std::string &UID, std::string &PW, std::string &UN); // register
  void pw(std::string &UID, std::string &CP, std::string &NP); // passwd
  void pw(std::string &UID, std::string &NP); // Upper User
  void ua(std::string &UID, std::string &PW, int PV, std::string &UN); // user add
  void de(std::string &UID); // delete
// above are functions about accounts management
// below are functions about bookshelf management
  void show();
  void showISBN(std::string &isbn);
  void showBookName(std::string &bookname);
  void showAuthor(std::string &authorname);
  void showKeyword(std::string &key);
  void buy(std::string &ISBN, int quan);
  void select(std::string &isbn);
  void modify(Book_Information &b);
  void import(int quan, double totalcost);
// above are functions about bookshelf management
// below are functions about log management
};

extern std::vector<User> stack; // log_in stack
// Each operation only operates on the user at the end of the queue.

#endif //CODE_ACCOUNT_H
