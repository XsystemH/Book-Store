//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_LOG_H
#define CODE_LOG_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include "book.h"
#include <ctime>

class financial {
public:

  std::fstream Flog;
  int logNum;
  double totalIn;
  double totalOut;

public:
  financial() {
    Flog.open("financial_log");
    if (!Flog.good()) {
      Flog.open("financial_log", std::ios::out | std::ios::binary);
      Flog.close();
      Flog.open("financial_log");
      logNum = 0;
      totalIn = 0.000;
      totalOut = 0.000;
      Flog.seekp(0, std::ios::beg);
      Flog.write(reinterpret_cast<char*>(&logNum), sizeof(int));
      Flog.write(reinterpret_cast<char*>(&totalIn), sizeof(double));
      Flog.write(reinterpret_cast<char*>(&totalOut), sizeof(double));
    }
    else {
      Flog.seekg(0, std::ios::beg);
      Flog.read(reinterpret_cast<char*>(&logNum), sizeof(int));
      Flog.read(reinterpret_cast<char*>(&totalIn), sizeof(double));
      Flog.read(reinterpret_cast<char*>(&totalOut), sizeof(double));
    }
    Flog.close();
  }
  ~financial() = default;

public:

  void recordin(double money) {
    totalIn += money;
    Flog.open("financial_log");
    Flog.seekp(sizeof(int) + (logNum + 2) * sizeof(double), std::ios::beg);
    Flog.write(reinterpret_cast<char*>(&money), sizeof(double));
    logNum++;
    Flog.seekp(0, std::ios::beg);
    Flog.write(reinterpret_cast<char*>(&logNum), sizeof(int));
    Flog.seekp(sizeof(int), std::ios::beg);
    Flog.write(reinterpret_cast<char*>(&totalIn), sizeof(double));
    Flog.close();
  }
  void recordout(double money) {
    totalOut += money;
    Flog.open("financial_log");
    money = -money; // - means out
    Flog.seekp(sizeof(int) + (logNum + 2) * sizeof(double), std::ios::beg);
    Flog.write(reinterpret_cast<char*>(&money), sizeof(double));
    logNum++;
    Flog.seekp(0, std::ios::beg);
    Flog.write(reinterpret_cast<char*>(&logNum), sizeof(int));
    Flog.seekp(sizeof(int) + sizeof(double), std::ios::beg);
    Flog.write(reinterpret_cast<char*>(&totalOut), sizeof(double));
    Flog.close();
  }
  void show() {
    std::cout << "+ ";
    std::cout << std::fixed << std::setprecision(2) << totalIn << " ";
    std::cout << "- ";
    std::cout << std::fixed << std::setprecision(2) << totalOut << "\n";
  }
  void show(int c) {
    if (c == 0) {
      std::cout << "\n";
      return;
    }
    if (c > logNum) {
      std::cout << "Invalid\n";
      return;
    }
    double in = 0;
    double out = 0;
    Flog.open("financial_log");
    for (int i = 0; i < c; i++) {
      double t = 0;
      Flog.seekg(sizeof(int) + (logNum - c + 2 + i) * sizeof(double), std::ios::beg);
      Flog.read(reinterpret_cast<char*>(&t), sizeof(double));
      if (t >= 0) in += t;
      else out -= t;
//      std::cout << std::fixed << std::setprecision(2) << t << "\t";
    }
    Flog.close();
    std::cout << "+ ";
    std::cout << std::fixed << std::setprecision(2) << in << " ";
    std::cout << "- ";
    std::cout << std::fixed << std::setprecision(2) << out << "\n";
  }
};

extern financial flog;

struct op {
  char UserID[65];
  int UserPV;
  int operation; // 1 on account; 2 on book
  char ISBN[65];
  char ID[65];
  char CMD[20];
  int num = -1;
  double pirce = 0.00;
  char Time[40];
  Book_Information after;

  void Print() {
    std::cout << "UserID: " << UserID << "\t";
    std::cout << "Privilege: " << UserPV << "\t";
    std::cout << "CMD: " << CMD << "\t";
    if (operation == 2) {
      std::cout << "ISBN(or else): " << ISBN << "\t";
    }
    if (operation == 1) {
      std::cout << "ID: " << ID << "\t";
    }
    if (num != -1) std::cout << num << "\t";
    std::cout << "At: " << Time << std::endl;
    if (operation == 2) {
      std::cout << "After operation: ";
      after.Print();
    }
  }
};

class log {
public:

  int lognum;
  std::fstream Log;

public:

  log() {
    Log.open("operation_log");
    if (!Log.good()) {
      Log.open("operation_log", std::ios::out | std::ios::binary);
      Log.close();
      Log.open("operation_log");
      lognum = 0;
      Log.seekp(0, std::ios::beg);
      Log.write(reinterpret_cast<char*>(&lognum), sizeof(int));
    }
    else {
      Log.seekg(0, std::ios::beg);
      Log.read(reinterpret_cast<char*>(&lognum), sizeof(int));
    }
  }
  ~log() = default;

public:

  const int MAX_TIMESTAMP_LENGTH = 26;

  void getCurrentTimestampAsCString(char* timestampStr) {
    // 获取当前时间戳
    time_t timestamp = time(nullptr);

    // 将时间戳转换为C风格字符串
    std::strftime(timestampStr, MAX_TIMESTAMP_LENGTH, "%Y-%m-%d %H:%M:%S", std::localtime(&timestamp));
  }

  void addlog(int OPER, std::string &UID, int PV, std::string Object, std::string CMD, Book_Information &after, double price) {
    op newoperation;
    getCurrentTimestampAsCString(newoperation.Time);
    newoperation.operation = OPER;
    strcpy(newoperation.UserID, UID.c_str());
    newoperation.UserPV = PV;
    if (OPER == 1) {
      strcpy(newoperation.ID, Object.c_str());
      strcpy(newoperation.ISBN, "");
    }
    else if (OPER == 2) {
      strcpy(newoperation.ISBN, Object.c_str());
      strcpy(newoperation.ID, "");
      newoperation.after = after;
    }
    strcpy(newoperation.CMD, CMD.c_str());
    newoperation.num = -1;
    newoperation.pirce = price;
    Log.open("operation_log");
    Log.seekp(sizeof(int) + lognum * sizeof(op), std::ios::beg);
    Log.write(reinterpret_cast<char*>(&newoperation), sizeof(op));
    lognum++;
    Log.seekp(0, std::ios::beg);
    Log.write(reinterpret_cast<char*>(&lognum), sizeof(int));
    Log.close();
  }
  void addlog(int OPER, std::string &UID, int PV, std::string Object, std::string CMD, Book_Information &after, int num) {
    op newoperation;
    getCurrentTimestampAsCString(newoperation.Time);
    newoperation.operation = OPER;
    strcpy(newoperation.UserID, UID.c_str());
    newoperation.UserPV = PV;
    if (OPER == 1) {
      strcpy(newoperation.ID, Object.c_str());
      strcpy(newoperation.ISBN, "");
    }
    else if (OPER == 2) {
      strcpy(newoperation.ISBN, Object.c_str());
      strcpy(newoperation.ID, "");
      newoperation.after = after;
    }
    strcpy(newoperation.CMD, CMD.c_str());
    newoperation.num = num;
    Log.open("operation_log");
    Log.seekp(sizeof(int) + lognum * sizeof(op), std::ios::beg);
    Log.write(reinterpret_cast<char*>(&newoperation), sizeof(op));
    lognum++;
    Log.seekp(0, std::ios::beg);
    Log.write(reinterpret_cast<char*>(&lognum), sizeof(int));
    Log.close();
  }

  void showall() {
    Log.open("operation_log");
    Log.seekg(sizeof(int), std::ios::beg);
    op temp;
    for (int i = 0; i < lognum; i++) {
      Log.read(reinterpret_cast<char*>(&temp), sizeof(op));
      temp.Print();
    }
  }
  void showemployee() {
    Log.open("operation_log");
    Log.seekg(sizeof(int), std::ios::beg);
    op temp;
    for (int i = 0; i < lognum; i++) {
      Log.read(reinterpret_cast<char*>(&temp), sizeof(op));
      if (temp.UserPV == 3) temp.Print();
    }
  }
  void showfinance() {
    Log.open("operation_log");
    Log.seekg(sizeof(int), std::ios::beg);
    op temp;
    for (int i = 0; i < lognum; i++) {
      Log.read(reinterpret_cast<char*>(&temp), sizeof(op));
      if (std::string(temp.CMD) == "buy") {
        std::cout << "\033[31m" << std::fixed << std::setprecision(2) << temp.pirce << "\033[0m\t";
        temp.Print();
      }
      else if (std::string(temp.CMD) == "import") {
        std::cout << "\033[32m" << std::fixed << std::setprecision(2) << temp.pirce << "\033[0m\t";
        temp.Print();
      }
    }
  }
};

extern log oplog;

#endif //CODE_LOG_H
