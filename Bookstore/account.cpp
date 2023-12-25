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
    std::cout << "Invalid\n";
    return;
  }
  User ready(*result.begin());
  if (PW == ready.Password) {
    stack.push_back(ready);
  }
  else {
    std::cout << "Invalid\n";
    return;
  }
}
void User::su(std::string &UID) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty())
  {
    std::cout << "Invalid\n";
    return;
  }
  if (Check(result.begin()->pos)) {
    User ready(*result.begin());
    stack.push_back(ready);
  }
  else {
    std::cout << "Invalid\n";
    return;
  }
}
void User::lo(std::string &UID) {
  if (stack.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  stack.pop_back(); // Pop out the last element in the login stack.
}
void User::rg(std::string &UID, std::string &PW, std::string &UN) {
  // add user in the file
  // need privilege: 0
  if (!accounts.find(UID).empty()) {
    std::cout << "Invalid\n";
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
    std::cout << "Invalid\n";
    return;
  }
  if (std::string(result.begin()->Password) != CP) {
    std::cout << "Invalid\n";
    return;
  }
  accounts.deleteI(*result.begin());
  strcpy(result.begin()->Password, NP.c_str());
  accounts.insert(*result.begin());
}
void User::pw(std::string &UID, std::string &NP) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  if (!Check(7)) {
    std::cout << "Invalid\n";
    return;
  }
  accounts.deleteI(*result.begin());
  strcpy(result.begin()->Password, NP.c_str());
  accounts.insert(*result.begin());
}
void User::ua(std::string &UID, std::string &PW, int PV, std::string &UN) {
  if (!Check(PV + 1)) {
    std::cout << "Invalid\n";
    return;
  }
  if (!accounts.find(UID).empty()) {
    std::cout << "Invalid\n";
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
    std::cout << "Invalid\n";
    return;
  }
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  for (User t : stack) {
    if (t.UserID == UID) {
      std::cout << "Invalid\n";
      return;
    }
  }
  accounts.deleteI(*result.begin());
}
// above are functions about accounts management
// below are functions about bookshelf management
void User::show() {
  if (!Check(1)) {
    std::cout << "Invalid\n";
    return;
  }
  BS.FindAll();
}
void User::showISBN(std::string &isbn) {
  if (!Check(1)) {
    std::cout << "Invalid\n";
    return;
  }
  BS.FindISBN(isbn);
}
void User::showBookName(std::string &bookname) {
  if (!Check(1)) {
    std::cout << "Invalid\n";
    return;
  }
  BS.FindName(bookname);
}
void User::showAuthor(std::string &authorname) {
  if (!Check(1)) {
    std::cout << "Invalid\n";
    return;
  }
  BS.FindAuthor(authorname);
}
void User::showKeyword(std::string &key) {
  if (!Check(1)) {
    std::cout << "Invalid\n";
    return;
  }
  BS.FIndKeyword(key);
}
void User::buy(std::string &ISBN, int quan) {
  if (!Check(1)) {
    std::cout << "Invalid\n";
    return;
  }
  std::vector<ISBN_info> result = BS.ISBN_chain->find(ISBN);
  if (result.empty()) {
    std::cout << "Invalid\n";
    return;
  }
  Book_Information book;
  BS.shelf.open("SHELF");
  BS.shelf.seekg(result.begin()->pos, std::ios::beg);
  BS.shelf.read(reinterpret_cast<char*>(&book), sizeof(Book_Information));
  BS.shelf.close();
  if (book.Quantity < quan) {
    std::cout << "Invalid\n";
    return;
  }
  book.Quantity -= quan;
  // todo log
  flog.recordin(quan * book.Price);
  std::cout << std::fixed << std::setprecision(2) << quan * book.Price << "\n";
  BS.shelf.open("SHELF");
  BS.shelf.seekp(result.begin()->pos, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&book), sizeof(Book_Information));
  BS.shelf.close();
}
void User::select(std::string &isbn) {
  if (!Check(3)) {
    std::cout << "Invalid\n";
    return;
  }
  selected = isbn;
  std::vector<ISBN_info> result = BS.ISBN_chain->find(isbn);
  if (result.empty()) {
    real = true;
    theBook = Book_Information(isbn);
    BS.InsertBook(theBook);
  }
  else {
    real = true;
    BS.shelf.open("SHELF");
    BS.shelf.seekg(result.begin()->pos, std::ios::beg);
    BS.shelf.read(reinterpret_cast<char*>(&theBook), sizeof(Book_Information));
    BS.shelf.close();
//    theBook.Print();
  }
}
void User::modify(Book_Information &b) {
  if (!Check(3)) {
    std::cout << "Invalid\n";
    return;
  }
  if (b.ISBN[0] == '\0') strcpy(b.ISBN, theBook.ISBN);
  if (b.BookName[0] == '\0') strcpy(b.BookName, theBook.BookName);
  if (b.AuthorName[0] == '\0') strcpy(b.AuthorName, theBook.AuthorName);
  if (b.Keywords[0] == '\0') strcpy(b.Keywords, theBook.Keywords);
  b.pos = theBook.pos;
  if (b.Quantity == 0) b.Quantity = theBook.Quantity;
  if (b.Price < 0) b.Price = theBook.Price;
  if (b.TotalCost < 0) b.TotalCost = theBook.TotalCost;
  // Filter out information that does not require replacement.
  try {
    BS.Modify(theBook, b);
  } catch (...) {
    std::cout << "Invalid\n";
    return;
  }
  theBook = b;
}
void User::import(int quan, double totalcost) {
  if (!Check(3)) {
    std::cout << "Invalid\n";
    return;
  }
  if (selected == "") {
    std::cout << "Invalid\n";
    return;
  }
  if (!real) {
    std::cout << "Invalid\n";
    return;
  }
  theBook.Quantity += quan;
  BS.shelf.open("SHELF");
  BS.shelf.seekp(theBook.pos, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&theBook), sizeof(Book_Information));
  BS.shelf.close();
  // todo log
  flog.recordout(totalcost);
}