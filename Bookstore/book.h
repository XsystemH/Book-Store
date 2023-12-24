//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_BOOK_H
#define CODE_BOOK_H

#include "blockchain.h"
#include <cstring>
#include <fstream>
#include <iomanip>

class Book_Information {
public:

  char ISBN[21];
  char BookName[65];
  char AuthorName[65];
  char Keywords[65];
  double Price;
  int Quantity; // Special Note: Values do not exceed 2'147'483'647
  double TotalCost;
  // Book Information
  // Storage Information
  int pos; // the position on the bookshelf

public:

  Book_Information() = default;
  Book_Information(std::string isbn) { // create a null book
    strcpy(ISBN, isbn.c_str());
    strcpy(BookName, "");
    strcpy(Keywords, "");
    Price = -1;
    Quantity = -1;
    TotalCost = -1;
    pos = -1;
  }
  ~Book_Information() = default;

  void Print() {
// [ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[库存数量]\n
    std::cout << ISBN << "\t" << BookName << "\t" << Keywords << "\t";
    std::cout << std::fixed << std::setprecision(2) << Price << "\t";
    std::cout << Quantity << '\n';
  }
};

class ISBN_node {
public:

  char maxn[21];
  char minn[21]; // ISBN最大长度20
  int pos;
  int next;

public:

  ISBN_node() {
    strcpy(maxn, "");
    strcpy(minn, "");
    pos = 0;
    next = -1;
  }
  ~ISBN_node() = default;
};

class ISBN_info {
public:

  char index[21]; // i.e ISBN
  char value[21]; //still ISBN
  int pos; // i.e. the position in origin file

public:

  ISBN_info() = default;
  ISBN_info(Book_Information &b) {
    strcpy(index, b.ISBN);
    strcpy(value, b.ISBN);
    pos = b.pos;
  }

  ISBN_info& operator=(const ISBN_info &b) {
    if (&b == this) return *this;
    strcpy(index, b.index);
    strcpy(value, b.value);
    pos = b.pos;
    return *this;
  }
};

class Name_node {
public:

  char maxn[65];
  char minn[65]; // Name最大长度20
  int pos;
  int next;

public:

  Name_node() {
    strcpy(maxn, "");
    strcpy(minn, "");
    pos = 0;
    next = -1;
  }
  ~Name_node() = default;
};

class Name_info {
public:

  char index[65]; // i.e. Name
  char value[21]; // i.e. ISBN
  int pos; // i.e. the position in origin file

public:

  Name_info() = default;
  Name_info(Book_Information &b) {
    strcpy(index, b.BookName);
    strcpy(value, b.ISBN);
    pos = b.pos;
  }

  Name_info& operator=(const Name_info &b) {
    if (&b == this) return *this;
    strcpy(index, b.index);
    strcpy(value, b.value);
    pos = b.pos;
    return *this;
  }
};

class Author_node {
public:

  char maxn[65];
  char minn[65]; // AuthorName最大长度20
  int pos;
  int next;

public:

  Author_node() {
    strcpy(maxn, "");
    strcpy(minn, "");
    pos = 0;
    next = -1;
  }
  ~Author_node() = default;
};

class Author_info {
public:

  char index[65]; // i.e. Author
  char value[21]; // i.e. ISBN
  int pos; // i.e. the position in origin file

public:

  Author_info() = default;
  Author_info(Book_Information &b) {
    strcpy(index, b.AuthorName);
    strcpy(value, b.ISBN);
    pos = b.pos;
  }

  Author_info& operator=(const Author_info &b) {
    if (&b == this) return *this;
    strcpy(index, b.index);
    strcpy(value, b.value);
    pos = b.pos;
    return *this;
  }
};

class Keyword_node {
public:

  char maxn[65];
  char minn[65]; // KeywordName最大长度20
  int pos;
  int next;

public:

  Keyword_node() {
    strcpy(maxn, "");
    strcpy(minn, "");
    pos = 0;
    next = -1;
  }
  ~Keyword_node() = default;
};

class Keyword_info {
public:

  char index[65]; // i.e. Keyword
  char value[21];  // i.e. ISBN
  int pos; // i.e. the position in origin file

public:

  Keyword_info() = default;
  Keyword_info(Book_Information &b, std::string key) {
    strcpy(index, key.c_str());
    strcpy(value, b.ISBN);
    pos = b.pos;
  }

  Keyword_info& operator=(const Keyword_info &b) {
    if (&b == this) return *this;
    strcpy(index, b.index);
    strcpy(value, b.value);
    pos = b.pos;
    return *this;
  }
};

// that's all blockchain we need in manage books
// ---------------------------------------------------------------------

class BookShelf {
public:

  blockchain<ISBN_node, ISBN_info> *ISBN_chain;
  blockchain<Name_node, Name_info> *Name_chain;
  blockchain<Author_node, Author_info> *Author_chain;
  blockchain<Keyword_node, Keyword_info> *Keyword_chain;

  std::string selected; // the ISBN of the selected Book;
  std::fstream shelf; // linear struct
  size_t BOOKSIZE;
  int booksum;
// the selected book's ISBN size - 21
// followed by all the books' information
// [ISBN]：Internal Strange Book Number；
// 合法字符集：除不可见字符以外 ASCII 字符；
// 最大长度：20；
// 特殊说明： 本系统中任何两本图书不应有相同 [ISBN] 信息。
// [BookName], [Author]：图书名字，图书作者名字；
// 合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
// 最大长度：60。
// [Keyword]：图书关键词；
// 合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
// 最大长度：60；
// 特殊说明：[keyword] 内容以 | 为分隔可以出现多段信息。如 math|magic|quantum 表示该图书具有三个无顺序的关键词 math, magic 和 quantum，每段信息长度至少为 1。
// [Quantity]：购买数量；
// 合法字符集：数字；
// 最大长度：10；
// 特殊说明：数值不超过 2'147'483'647。
// [Price], [TotalCost]：图书单价，交易总额。
// 合法字符集：数字和 .；
// 最大长度：13；
// 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位。

public:

  BookShelf() {
    ISBN_chain = new blockchain<ISBN_node, ISBN_info>("ISBN_node", "ISBN_info");
    Name_chain = new blockchain<Name_node, Name_info>("Name_node", "Name_info");
    Author_chain = new blockchain<Author_node, Author_info>("Author_node", "Author_info");
    Keyword_chain = new blockchain<Keyword_node, Keyword_info>("Keyword_node", "Ketword_info");
    shelf.open("SHELF");
    if (!shelf.good()) {
      shelf.open("SHELF", std::ios::out | std::ios::binary);
      shelf.close();
      shelf.open("SHELF");
      booksum = 0; // initialize
      shelf.seekp(0, std::ios::beg);
      shelf.write(reinterpret_cast<char*>(&booksum), sizeof(int));
    }
    else {
      shelf.seekg(0, std::ios::beg);
      shelf.read(reinterpret_cast<char*>(&booksum), sizeof(int));
      // get information if file is created already
    }
    shelf.close(); // 创建文件
    BOOKSIZE = sizeof(Book_Information);
  }
  ~BookShelf() {
    delete ISBN_chain;
    delete Name_chain;
    delete Author_chain;
    delete Keyword_chain;
  }

  // std::vector<Book_Information>
  void FindAll();
  void FindISBN(std::string &isbn); // maybe void is better?
  void FindName(std::string &Aname);
  void FindAuthor(std::string &Aname);
  void FIndKeyword(std::string &Word);

/**
 * @brief insert a book
 * @details Write information into SHELF
 * and create index of all types pointing to pos.
 * remember to record pos in book
 * @param book the book need to be insert
 */
  void InsertBook(Book_Information &book);
/**
 * @brief delete a book
 * @details delete information from SHELF
 * and delete all types pointing to pos.
 * remember to record pos in book
 * @param book the book need to be delete
 */
  void DeleteBook(Book_Information &book);
/**
 * @brief change the information of a book
 * @details Modify the information in its original position.
 * Delete all existing indexes.
 * Create new indexes.
 */
  void Modify(Book_Information &oldbook, Book_Information &newbook);

};

BookShelf BS;

#endif //CODE_BOOK_H
