//
// Created by 13199 on 2023/12/12.
//
#include "1336.hpp"
#include "blockchain.h"
#include <iostream>
#include <string>
#include <vector>

class node
{
public:

  std::string maxn;
  std::string minn;
  int pos;
  int next;
  int last;

public:

  node()
  {
    maxn = std::string(64, ' ');
    minn = std::string(64, ' ');
  }
  ~node() = default;
};

class info
{
public:
  std::string index;
  int value;
public:
  info() = default;
  info(std::string i, int v)
  {
    index = i;
    value = v;
  }
  ~info() = default;
};

int main()
{
  blockchain<node, info> mychain("NODE", "INFO");
  int n;
  std::cin >> n;
  for (int i = 0; i < n; i++)
  {
    std::string command, index;
    int value;
    std::cin >> command >> index;
    if (command == "insert")
    {
      std::cin >> value;
      info temp(index, value);
      mychain.insert(temp);
    }
    else if (command == "delete")
    {
      std::cin >> value;
      // mychain.delete(index, value);
    }
    else if (command == "find")
    {
      std::vector<std::pair<int, int>> poss = mychain.findN(index);
      std::vector<int> result = mychain.findI(poss, index);
      if (result.empty())
      {
        std::cout << "null\n";
      }
      else
      {
        for (int val : result) {
          std::cout << val << " ";
        }
        std::cout << "\n";
      }
    }
  }
  return 0;
}