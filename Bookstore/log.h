//
// Created by 13199 on 2023/12/21.
//

#ifndef CODE_LOG_H
#define CODE_LOG_H

#include <fstream>

class financial {
public:

  std::fstream flog;
  int logNum;
  double totalIn;
  double totalOut;

public:
  financial() {
    flog.open("financial_log");
    if (!flog.good()) {
      flog.open("financial_log", std::ios::out | std::ios::binary);
      logNum = 0;
      totalIn = 0.000;
      totalOut = 0.000;
      flog.seekp(0, std::ios::beg);
      flog.write(reinterpret_cast<char*>(&logNum), sizeof(int));
      flog.write(reinterpret_cast<char*>(&totalIn), sizeof(double));
      flog.write(reinterpret_cast<char*>(&totalOut), sizeof(double));
    }
    else {
      flog.seekg(0, std::ios::beg);
      flog.read(reinterpret_cast<char*>(&logNum), sizeof(int));
      flog.read(reinterpret_cast<char*>(&totalIn), sizeof(double));
      flog.read(reinterpret_cast<char*>(&totalOut), sizeof(double));
    }
    flog.close();
  }
  ~financial() = default;

public:

  void recordin(double money) {
    totalIn += money;
    flog.open("financial_log");
    flog.seekp(sizeof(int) + (logNum + 2) * sizeof(double), std::ios::beg);
    flog.write(reinterpret_cast<char*>(&money), sizeof(double));
    logNum++;
    flog.seekp(0, std::ios::beg);
    flog.write(reinterpret_cast<char*>(&logNum), sizeof(int));
    flog.write(reinterpret_cast<char*>(&totalIn), sizeof(double));
    flog.close();
  }
  void recordout(double money) {
    totalOut += money;
    flog.open("financial_log");
    money = -money; // - means out
    flog.seekp(sizeof(int) + (logNum + 2) * sizeof(double), std::ios::beg);
    flog.write(reinterpret_cast<char*>(&money), sizeof(double));
    logNum++;
    flog.seekp(0, std::ios::beg);
    flog.write(reinterpret_cast<char*>(&logNum), sizeof(int));
    flog.seekp(sizeof(int) + sizeof(double), std::ios::beg);
    flog.write(reinterpret_cast<char*>(&totalOut), sizeof(double));
    flog.close();
  }
  void show() {
    std::cout << "+ ";
    std::cout << std::fixed << std::setprecision(2) << totalIn << " ";
    std::cout << "- ";
    std::cout << std::fixed << std::setprecision(2) << totalOut << "\n";
  }
  void show(int count) {
    if (count == 0) {
      std::cout << "\n";
      return;
    }
    if (count > logNum) {
      std::cout << "Invalid\n";
      return;
    }
    double in = 0;
    double out = 0;
    flog.open("financial_log");
    for (int i = 0; i < count; i++) {
      double t = 0;
      flog.seekg(sizeof(int) + (logNum - count + 2 + i) * sizeof(double), std::ios::beg);
      flog.read(reinterpret_cast<char*>(&t), sizeof(double));
      if (t >= 0) in += t;
      else out -= t;
//      std::cout << std::fixed << std::setprecision(2) << t << "\t";
    }
    flog.close();
    std::cout << "+ ";
    std::cout << std::fixed << std::setprecision(2) << in << " ";
    std::cout << "- ";
    std::cout << std::fixed << std::setprecision(2) << out << "\n";
  }
};

extern financial flog;

#endif //CODE_LOG_H
