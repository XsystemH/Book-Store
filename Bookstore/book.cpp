//
// Created by 13199 on 2023/12/21.
//
#include "book.h"

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

void BookShelf::select(std::string isbn) {
  selected = isbn;
}