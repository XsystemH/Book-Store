//
// Created by 13199 on 2023/12/21.
//
#include "book.h"
#include <algorithm>

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
}

void BookShelf::FIndKeyword(std::string &key) {
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
}
// above are functions related to finding a list of book
void BookShelf::InsertBook(Book_Information &book) {

}
void BookShelf::DeleteBook(Book_Information &book) {

}
void BookShelf::Modify(Book_Information &oldbook, Book_Information &newbook) {

}