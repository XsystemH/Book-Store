//
// Created by 13199 on 2023/12/24.
//
#include "account.h"
#include "blockchain.h"
#include "log.h"
#include <iostream>
#include <string>
#include <cstring>
#include "grammar.h"

BookShelf BS;
blockchain<Account_node, Account_info> accounts("Account_NODE", "Account_INFO");
std::vector<User> stack;
financial flog;

int main() {
  if (accounts.empty()) {
    User root;
    root.UserID = "root";
    root.Password = "sjtu";
    root.Privilege = 7;
    Account_info r;
    strcpy(r.index, "root"); // ID
    strcpy(r.value, "");     // Name
    strcpy(r.Password, "sjtu");// PW
    r.pos = 7; // privilege
    accounts.insert(r);
  } // create root
  char cmd[1000];
//  int test = 1;
  while (std::cin.getline(cmd, 1000)) {
//    std::cout << test << ": ";
//    test++;
//    if (test == 29) {
//      test--;
//      test++;
//    }
    if (cmd[0]=='\0') {
      continue;
    }
    if (stack.empty()) {
      User visitor;
      visitor.Privilege = 0;
      stack.push_back(visitor);
    }
    char ignore[] = " ";
    char* tmp = strtok(cmd, ignore);
    std::vector<std::string> cut;
    while (tmp) {
      if (*tmp != '\0') {
        cut.emplace_back(tmp);
      }
      tmp = strtok(nullptr, ignore);
    } // Slice cmd
    if (cut[0] == "su") {
      if (cut.size() < 2) {
        std::cout << "Invalid\n";
        continue;
      }
      else if (cut.size() == 2) {
        stack.back().su(cut[1]);
      }
      else if (cut.size() == 3) {
        stack.back().su(cut[1], cut[2]);
      }
      else {
        std::cout << "Invalid\n";
        continue;
      }
    }
    else if (cut[0] == "logout") {
      if (cut.size() == 1 && stack.back().Privilege) stack.pop_back();
      else {
        std::cout << "Invalid\n";
        continue;
      }
      if (stack.back().selected != "") {
        std::vector<ISBN_info> result = BS.ISBN_chain->find(stack.back().selected);
        if (result.empty()) {
          stack.back().real = true;
          stack.back().theBook = Book_Information(stack.back().selected);
          BS.InsertBook(stack.back().theBook);
        }
        else {
          stack.back().real = true;
          BS.shelf.open("SHELF");
          BS.shelf.seekg(result.begin()->pos, std::ios::beg);
          BS.shelf.read(reinterpret_cast<char*>(&stack.back().theBook), sizeof(Book_Information));
          BS.shelf.close();
//    theBook.Print();
        }
      } // renew the book when log out
    }
    else if (cut[0] == "register") {
      if (cut.size() != 4) {
        std::cout << "Invalid\n";
        continue;
      }
      if (checkNLU(cut[1]) && checkNLU(cut[2]))
        stack.back().rg(cut[1], cut[2], cut[3]);
      else {
        std::cout << "Invalid\n";
        continue;
      }
    }
    else if (cut[0] == "passwd") {
      if (cut.size() < 3) {
        std::cout << "Invalid\n";
        continue;
      }
      else if (cut.size() == 3) {
        if (checkNLU(cut[1]) && checkNLU(cut[2]))
          stack.back().pw(cut[1], cut[2]);
        else {
          std::cout << "Invalid\n";
          continue;
        }
      }
      else if (cut.size() == 4) {
        if (checkNLU(cut[1]) && checkNLU(cut[2]) && checkNLU(cut[3]))
          stack.back().pw(cut[1], cut[2], cut[3]);
        else {
          std::cout << "Invalid\n";
          continue;
        }
      }
    }
    else if (cut[0] == "useradd") {
      if (cut.size() != 5) {
        std::cout << "Invalid\n";
        continue;
      }
      if (cut[3].length() > 1) {
        std::cout << "Invalid\n";
        continue;
      }
      int privilege = int(cut[3][0] - '0');
      if (checkNLU(cut[1]) && checkNLU(cut[2]))
        stack.back().ua(cut[1], cut[2], privilege, cut[4]);
      else {
        std::cout << "Invalid\n";
        continue;
      }
    }
    else if (cut[0] == "delete") {
      if (cut.size() != 2) {
        std::cout << "Invalid\n";
        continue;
      }
      stack.back().de(cut[1]);
    }
    else if (cut[0] == "show") { // book or finance
      if (cut.size() == 1) {
        stack.back().show();
      }
      else if (cut[1] == "finance") {
        if (stack.back().Check(7))
        if (cut.size() == 2) {
          flog.show();
        }
        else if (cut.size() == 3) {
          try {
            int c = std::stoi(cut[2]);
            flog.show(c);
          } catch(...) {
            std::cout << "Invalid\n";
            continue;
          }
        }
        else {
          std::cout << "Invalid\n";
          continue;
        }
      }
      else {
        if (cut[1][1] == 'I') {
          std::string isbn = cut[1].substr(6);
          stack.back().showISBN(isbn);
        }
        else if (cut[1][1] == 'n') {
          std::string name = cut[1].substr(7,cut[1].length() - 8);
          stack.back().showBookName(name);
        }
        else if (cut[1][1] == 'a') {
          std::string Aname = cut[1].substr(9,cut[1].length() - 10);
          stack.back().showAuthor(Aname);
        }
        else if (cut[1][1] == 'k') {
          std::string key = cut[1].substr(10,cut[1].length() - 11);
          stack.back().showKeyword(key);
        }
        else {
          std::cout << "Invalid\n";
          continue;
        }
      } // if show book
    } // if show
    else if (cut[0] == "buy") {
      if (cut.size() != 3) {
        std::cout << "Invalid\n";
        continue;
      }
      int q = 0;
      bool isint = true;
      for(char c : cut[2]) {
        if (c == '.') {
          std::cout << "Invalid\n";
          isint = false;
        }
        q *= 10;
        q += int(c - '0');
      }
      if (!isint) {
        std::cout << "Invalid\n";
        continue;
      }
      if (q == 0) {
        std::cout << "Invalid\n";
        continue;
      }
      stack.back().buy(cut[1], q);
    }
    else if (cut[0] == "select") {
      if (cut.size() != 2) {
        std::cout << "Invalid\n";
        continue;
      }
      stack.back().select(cut[1]);
    }
    else if (cut[0] == "modify") {
      if (stack.back().selected == "") {
        std::cout << "Invalid\n";
        continue;
      }
      Book_Information change(stack.back().selected);
      try {
        for (int i = 1; i < cut.size(); i++) {
          if (cut[i][1] == 'I') {
            std::string isbn = cut[1].substr(6);
            if (isbn == stack.back().selected) {
              std::cout << "Invalid\n";
              throw "";
            }
            if (!BS.ISBN_chain->find(isbn).empty()) {
              throw error();
            }
            strcpy(change.ISBN, isbn.c_str());
            stack.back().selected = isbn;
          } else if (cut[i][1] == 'n') {
            std::string name = cut[i].substr(7, cut[i].length() - 8);
            strcpy(change.BookName, name.c_str());
          } else if (cut[i][1] == 'a') {
            std::string Aname = cut[i].substr(9, cut[i].length() - 10);
            strcpy(change.AuthorName, Aname.c_str());
          } else if (cut[i][1] == 'k') {
            std::string key = cut[i].substr(10, cut[i].length() - 11);
            strcpy(change.Keywords, key.c_str());
          } else if (cut[i][1] == 'p') {
            try {
              std::string p = cut[i].substr(7);
              change.Price = std::stod(p);
            } catch (std::invalid_argument &error) {
              std::cout << "Invalid\n";
            } catch (std::out_of_range &error) {
              std::cout << "Invalid\n";
            }
          } else {
            std::cout << "Invalid\n";
            throw error();
          }
        }
      } catch (...) {
        std::cout << "Invalid\n";
        continue;
      }
      stack.back().modify(change);
    }
    else if (cut[0] == "import") {
      if (cut.size() != 3) {
        std::cout << "Invalid\n";
        continue;
      }
      try {
        int q = std::stoi(cut[1]);
        double total = std::stod(cut[2]);
        stack.back().import(q, total);
      } catch (std::invalid_argument &e) {
        std::cout << "Invalid\n";
      } catch (std::out_of_range &e) {
        std::cout << "Invalid\n";
      }
    }
    else if (cut[0] == "log") {
      // todo
    }
    else if (cut[0] == "report") {
      // todo
    }
    else if (cut[0] == "quit" || cut[0] == "exit") {
      break;
    }
    else {
      std::cout << "Invalid\n";
    }
  }
  return 0;
}