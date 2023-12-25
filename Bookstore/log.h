//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_LOG_H
#define CODE_LOG_H

#include <fstream>
#include <iostream>
#include <iomanip>

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

#endif //CODE_LOG_H
