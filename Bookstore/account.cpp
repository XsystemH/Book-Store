//
// Created by 13199 on 2023/12/21.
//
#include "account.h"

bool User::Check(int need) {
  if (Privilege < need) return false;
  else return true;
}
void User::su(std::string &UID, std::string &PW) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty())
  {
    // todo error
    return;
  }
  User ready(*result.begin());
  if (PW == ready.Password) {
    stack.push_back(ready);
  }
  else {
    // todo error
    return;
  }
}
void User::su(std::string &UID) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty())
  {
    // todo error
    return;
  }
  if (Check(result.begin()->pos)) {
    User ready(*result.begin());
    stack.push_back(ready);
  }
  else {
    // todo error
    return;
  }
}
void User::lo(std::string &UID) {
  if (stack.empty()) {
    // todo error
    return;
  }
  stack.pop_back(); // Pop out the last element in the login stack.
}
void User::rg(std::string &UID, std::string &PW, std::string &UN) {
  // add user in the file
  // need privilege: 0
  if (!accounts.find(UID).empty()) {
    // todo error
    return;
  }
  Account_info new_user;
  strcpy(new_user.index, UID.c_str());
  strcpy(new_user.value, UN.c_str());
  strcpy(new_user.Password, PW.c_str());
  new_user.pos = 1;
  accounts.insert(new_user);
}
void User::pw(std::string &UID, std::string &CP, std::string &NP) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    // todo error
    return;
  }
  if (std::string(result.begin()->Password) != CP) {
    // todo wrong password
    return;
  }
  accounts.deleteI(*result.begin());
  strcpy(result.begin()->Password, NP.c_str());
  accounts.insert(*result.begin());
}
void User::pw(std::string &UID, std::string &NP) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    // todo error
    return;
  }
  if (!Check(7)) {
    // todo privilege 7 is required
    return;
  }
  accounts.deleteI(*result.begin());
  strcpy(result.begin()->Password, NP.c_str());
  accounts.insert(*result.begin());
}
void User::ua(std::string &UID, std::string &PW, int PV, std::string &UN) {
  if (!Check(PV)) {
    // todo higher privilege is required
    return;
  }
  if (!accounts.find(UID).empty()) {
    // todo user exists already
    return;
  }
  Account_info new_user;
  strcpy(new_user.index, UID.c_str());
  strcpy(new_user.value, UN.c_str());
  strcpy(new_user.Password, PW.c_str());
  new_user.pos = PV;
  accounts.insert(new_user);
}
void User::de(std::string &UID) {
  if (!Check(7)) {
    // todo need privilege 7
    return;
  }
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    // todo null
    return;
  }
  for (User t : stack) {
    if (t.UserID == UID) {
      // todo in stack
      return;
    }
  }
  accounts.deleteI(*result.begin());
}
// above are functions about accounts management
// below are functions about bookshelf management
void User::show() {
  if (!Check(1)) {
    // todo need higher privilege
    return;
  }
  BS.FindAll();
}
void User::showISBN(std::string &isbn) {
  if (!Check(1)) {
    // todo need higher privilege
    return;
  }
  BS.FindISBN(isbn);
}
void User::showBookName(std::string &bookname) {
  if (!Check(1)) {
    // todo need higher privilege
    return;
  }
  BS.FindName(bookname);
}
void User::showAuthor(std::string &authorname) {
  if (!Check(1)) {
    // todo need higher privilege
    return;
  }
  BS.FindAuthor(authorname);
}
void User::showKeyword(std::string &key) {
  if (!Check(1)) {
    // todo need higher privilege
    return;
  }
  BS.FIndKeyword(key);
}
void User::buy(std::string &ISBN, int quan) {
  if (!Check(1)) {
    // todo need higher privilege
    return;
  }
  std::vector<ISBN_info> result = BS.ISBN_chain->find(ISBN);
  Book_Information book;
  BS.shelf.open("SHELF");
  BS.shelf.seekg(result.begin()->pos, std::ios::beg);
  BS.shelf.read(reinterpret_cast<char*>(&book), sizeof(Book_Information));
  BS.shelf.close();
  if (book.Quantity < quan) {
    // todo Book is not enough
    return;
  }
  book.Quantity -= quan;
  // todo log
  BS.shelf.open("SHELF");
  BS.shelf.seekp(result.begin()->pos, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&book), sizeof(Book_Information));
  BS.shelf.close();
}
void User::select(std::string &isbn) {
  if (!Check(3)) {
    // todo higher privilege is required
    return;
  }
  selected = isbn;
  std::vector<ISBN_info> result = BS.ISBN_chain->find(isbn);
  if (result.empty()) {
    real = false;
    theBook = Book_Information(isbn);
  }
  else {
    real = true;
    BS.shelf.open("SHELF");
    BS.shelf.seekg(result.begin()->pos, std::ios::beg);
    BS.shelf.read(reinterpret_cast<char*>(&theBook), sizeof(Book_Information));
    BS.shelf.close();
  }
}
void User::modify(Book_Information &b) {
  if (!Check(3)) {
    // todo higher privilege is required
    return;
  }
  if (!real) {
    // Check integrity and insert the book
    if (b.ISBN[0] == '\0' || b.BookName[0] == '\0' || b.AuthorName[0] == '\0'
      || b.Keywords[0] == '\0' || b.Price < 0) {
      // todo information isn't complete
      return;
    }
    BS.InsertBook(b);
    return;
  }
  if (b.ISBN[0] == '\0') strcpy(b.ISBN, theBook.ISBN);
  if (b.BookName[0] == '\0') strcpy(b.BookName, theBook.BookName);
  if (b.AuthorName[0] == '\0') strcpy(b.AuthorName, theBook.AuthorName);
  if (b.Keywords[0] == '\0') strcpy(b.Keywords, theBook.Keywords);
  b.pos = theBook.pos;
  if (b.Quantity == -1) b.Quantity = theBook.Quantity;
  if (b.Price < 0) b.Price = theBook.Price;
  if (b.TotalCost == -1) b.TotalCost = theBook.TotalCost;
  // Filter out information that does not require replacement.
  BS.Modify(theBook, b);
}
void User::import(std::string &quan, double totalcost) {
  if (!Check(3)) {
    // todo
    return;
  }
  if (selected == "") {
    // todo
    return;
  }
  if (!real) {
    // todo
    return;
  }
  int q = 0;
  for (char c : quan) {
    if (c == '-' || c == '.') {
      // not an int number
      return;
    }
    q *= 10;
    q += int(c - '0');
  }
  theBook.Quantity -= q;
  BS.shelf.open("SHELF");
  BS.shelf.seekp(theBook.pos, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&theBook), sizeof(Book_Information));
  BS.shelf.close();
  // todo log
}