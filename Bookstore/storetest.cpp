//
// Created by 13199 on 2023/12/12.
//
#include "blockchain.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class node
{
public:

  char maxn[65];
  char minn[65];
  int pos;
  int next;
  int last;

public:

  node()
  {
    strcpy(maxn, "");
    strcpy(minn, "");
  }
  ~node() = default;
};

class info
{
public:
  char index[65]{};
  int value;
public:
  info() = default;
  info(std::string i, int v)
  {
    strcpy(index, i.c_str());
    value = v;
  }
  ~info() = default;
};

int main()
{
//  std::fstream f;
//  f.open("test.bin");
//  int tmp = 0;
//  if (!f.good())
//  {
//    f.open("test.bin", std::ios::out | std::ios::binary);
//    f.close();
//    f.open("test.bin");
//    f.write(reinterpret_cast<char *>(&tmp), sizeof(int));
//  }
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
      info temp(index, value);
      mychain.deleteI(temp);
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