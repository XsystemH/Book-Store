//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_BOOK_H
#define CODE_BOOK_H

#include "blockchain.h"
#include <cstring>
#include <fstream>

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
  char BookName[65]; // used when turning
  int pos; // i.e. the position in origin file

public:

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

  char index[65]; // i.e Name
  char value[21];  // i.e ISBN
  int pos; // i.e. the position in origin file

public:

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
  char value[21];  // i.e. ISBN
  int pos; // i.e. the position in origin file

public:

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

class Book_Information {
  char ISBN[21];
  char BookName[65];
  char Keywords[65];
  unsigned long long Quantity;
  double Price;
  double TotalCost;
};

class BookShelf {
public:

  blockchain<ISBN_node, ISBN_info> *ISBN_chain;
  blockchain<Name_node, Name_info> *Name_chain;
  blockchain<Author_node, Author_info> *Author_chain;
  blockchain<Keyword_node, Keyword_info> *Keyword_chain;

  std::string selected; // the ISBN of the selected Book;
  std::fstream shelf; // linear struct
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
  }
  ~BookShelf() {
    delete ISBN_chain;
    delete Name_chain;
    delete Author_chain;
    delete Keyword_chain;
  }

  void select(std::string isbn);
};

#endif //CODE_BOOK_H
