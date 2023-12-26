//
// Created by 13199 on 2023/12/21.
//
#include "book.h"
#include <algorithm>
#include <set>

//ISBN_info NametoISBN(const Name_info &b) {
//  ISBN_info a;
//  strcpy(a.index, b.value);
//  strcpy(a.value, b.index);
//  a.pos = b.pos;
//}
//Name_info ISBNtoName(const Name_info &b) {
//  Name_info a;
//  strcpy(a.index, b.value);
//  strcpy(a.value, b.index);
//  a.pos = b.pos;
//}
// useless because we don't need to modify name and ISBN

void BookShelf::FindAll() {
  std::vector<int> pos = ISBN_chain->findA();
  shelf.open("SHELF");
  for (int p : pos) {
    shelf.seekg(p, std::ios::beg);
    Book_Information book;
    shelf.read(reinterpret_cast<char*>(&book), BOOKSIZE);
    book.Print();
  }
  if (pos.empty()) std::cout << "\n";
  shelf.close();
}
void BookShelf::FindISBN(std::string &isbn) {
  std::vector<std::pair<int, int>> poss = ISBN_chain->findN(isbn);
  std::vector<int> pos = ISBN_chain->findI(poss, isbn);
  for (int p : pos) {
    shelf.open("SHELF");
    shelf.seekg(p, std::ios::beg);
    Book_Information book;
    shelf.read(reinterpret_cast<char*>(&book), BOOKSIZE);
    shelf.close();
    book.Print();
  }
  if (pos.empty()) std::cout << "\n";
} // Sort and output in ascending order by ISBN, no need for sorting.

bool cmp(Book_Information &a, Book_Information &b) {
  std::string astr = a.ISBN;
  std::string bstr = b.ISBN;
  return astr < bstr;
}

void BookShelf::FindName(std::string &name) {
  std::vector<std::pair<int, int>> poss = Name_chain->findN(name);
  std::vector<int> pos = Name_chain->findI(poss, name);
  std::vector<Book_Information> list;
  for (int p : pos) {
    shelf.open("SHELF");
    shelf.seekg(p, std::ios::beg);
    Book_Information book;
    shelf.read(reinterpret_cast<char*>(&book), BOOKSIZE);
    shelf.close();
    list.push_back(book);
  }
  std::sort(list.begin(), list.end(), cmp);
  for (Book_Information b : list) {
    b.Print();
  }
  if (pos.empty()) std::cout << "\n";
}

void BookShelf::FindAuthor(std::string &name) {
  std::vector<std::pair<int, int>> poss = Author_chain->findN(name);
  std::vector<int> pos = Author_chain->findI(poss, name);
  std::vector<Book_Information> list;
  for (int p : pos) {
    shelf.open("SHELF");
    shelf.seekg(p, std::ios::beg);
    Book_Information book;
    shelf.read(reinterpret_cast<char*>(&book), BOOKSIZE);
    shelf.close();
    list.push_back(book);
  }
  std::sort(list.begin(), list.end(), cmp);
  for (Book_Information b : list) {
    b.Print();
  }
  if (list.empty()) std::cout << "\n";
}

void BookShelf::FIndKeyword(std::string &key) {
  for (char c : key) {
    if (c == '|') {
      throw error();
    }
  }
  std::vector<std::pair<int, int>> poss = Keyword_chain->findN(key);
  std::vector<int> pos = Keyword_chain->findI(poss, key);
  std::vector<Book_Information> list;
  for (int p : pos) {
    shelf.open("SHELF");
    shelf.seekg(p, std::ios::beg);
    Book_Information book;
    shelf.read(reinterpret_cast<char*>(&book), BOOKSIZE);
    shelf.close();
    list.push_back(book);
  }
  std::sort(list.begin(), list.end(), cmp);
  for (Book_Information b : list) {
    b.Print();
  }
  if (list.empty()) std::cout << "\n";
}
// above are functions related to finding a list of book
void BookShelf::InsertBook(Book_Information &book) {
  int sum;
  BS.shelf.open("SHELF");
  BS.shelf.seekg(0, std::ios::beg);
  BS.shelf.read(reinterpret_cast<char*>(&sum), sizeof(int));
  book.pos = sizeof(int) + sum * BOOKSIZE;
  sum++;
  BS.shelf.seekp(0, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&sum), sizeof(int));
  BS.shelf.seekp(book.pos, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&book), BOOKSIZE);
  BS.shelf.close();
  ISBN_info isbn(book);
  ISBN_chain->insert(isbn);
  if (book.BookName[0] != '\0') {
    Name_info name(book);
    Name_chain->insert(name);
  }
  if (book.AuthorName[0] != '\0') {
    Author_info author(book);
    Author_chain->insert(author);
  }
  char keywords[65];
  strcpy(keywords, book.Keywords);
  char* key = strtok(keywords, "|");
  std::set<std::string> used;
  while (key) {
    if (used.find(key) != used.end()) {
      throw error();
    }
    used.emplace(key);
    Keyword_info kw(book, key);
    Keyword_chain->insert(kw);
    key = strtok(nullptr, "|");
  }
}
void BookShelf::DeleteBook(Book_Information &book) {
  ISBN_info isbn(book);
  ISBN_chain->deleteI(isbn);
  if (book.BookName[0] != '\0') {
    Name_info name(book);
    Name_chain->deleteI(name);
  }
  if (book.AuthorName[0] != '\0') {
    Author_info author(book);
    Author_chain->deleteI(author);
  }
  char keywords[65];
  strcpy(keywords, book.Keywords);
  char* key = strtok(keywords, "|");
  while (key) {
    Keyword_info kw(book, key);
    Keyword_chain->deleteI(kw);
    key = strtok(nullptr, "|");
  }
}
void BookShelf::Modify(Book_Information &oldbook, Book_Information &newbook) {
  DeleteBook(oldbook);
  ISBN_info isbn(newbook);
  ISBN_chain->insert(isbn);
  if (newbook.BookName[0] != '\0'){
    Name_info name(newbook);
    Name_chain->insert(name);
  }
  if (newbook.AuthorName[0] != '\0'){
    Author_info author(newbook);
    Author_chain->insert(author);
  }
  char keywords[65];
  strcpy(keywords, newbook.Keywords);
  char* key = strtok(keywords, "|");
  std::set<std::string> s;
  while (key) {
    if (s.find(std::string(key)) != s.end()) {
      throw error();
    }
    s.emplace(key);
    Keyword_info kw(newbook, key);
    Keyword_chain->insert(kw);
    key = strtok(nullptr, "|");
  }
  BS.shelf.open("SHELF");
  BS.shelf.seekp(newbook.pos, std::ios::beg);
  BS.shelf.write(reinterpret_cast<char*>(&newbook), BOOKSIZE);
  BS.shelf.close(); // Override old to new
}