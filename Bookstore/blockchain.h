#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

const size_t Max = 4096 * 50;

template<class node, class info>
class blockchain
{

private:

  int sum;
  size_t Nsize;
  size_t Isize;
  std::string NODE;
  std::string INFO;
  std::fstream Node;
  std::fstream Info;

public:

  blockchain(std::string Nname, std::string Iname)
  {
    sum = 0;
    NODE = Nname;
    INFO = Iname;
    Node.open(NODE, std::ios::in | std::ios::out | std::ios::binary);
    Info.open(INFO, std::ios::in | std::ios::out | std::ios::binary);
    Nsize = sizeof(node);
    Isize = sizeof(info);
    if (Node.peek() == std::ifstream::traits_type::eof())
    {
      node head;
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&head), Nsize);
    }
  }

  ~blockchain() = default;

  std::vector<int> findN(std::string *index)
  {
    std::vector<int> poss;
    node tpr;
    Node.seekg(0, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&tpr), Nsize);
    while (tpr.next != -1)
    {
      Node.seekg(tpr.next);
      if (index > tpr.maxn)
      {
        continue;
      }
      else if (tpr.minn <= index && index < tpr.maxn)
      {
        poss.push_back(tpr.pos);
        break;
      }
      else if (index == tpr.maxn) 
      {
        poss.push_back(tpr.pos);
      }
      else if (index < tpr.minn)
      {
        break;
      }
    }
    return poss;
  }

  info findI(std::vector<int> &poss, info target, int &pos)
  {
    int size;
    info val;
    for(int i = 0; i < poss.size(); i++)
    {
      Info.seekg(poss[i], std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      for(int i = 0; i <= size; i++)
      {
        Info.seekg(Isize, std::ios::cur);
        Info.read(reinterpret_cast<char*>(&val), Isize);
        pos = Info.tellg();
        if(val == target)
        {
          return val;
        }
        if(val.index == target.index && val.value > target.value)
        {
          return nullptr;
        }
      }
    }
    return nullptr;
  }

  std::vector<int> findI(std::vector<int> &poss, std::string index)
  {
    std::vector<int> vals;
    int size;
    info val;
    for(int i = 0; i < poss.size(); i++)
    {
      Info.seekg(poss[i], std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      for(int i = 0; i <= size; i++)
      {
        Info.seekg(Isize, std::ios::cur);
        Info.read(reinterpret_cast<char*>(&val), Isize);
        if(val.index == index)
        {
          vals.push_back(val.value);
        }
      }
    }
    return vals;
  }

  void deleteI(int pos0, int posI)
  {
    int size;
    info temp;
    Info.seekg(pos0, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&size), sizeof(int));
    Info.seekg(posI + Isize, std::ios::beg);
    Info.seekp(posI, std::ios::beg);
    for (int i = 1; i < size - (posI - pos0 + sizeof(int)) / Isize; i++)
    {
      Info.read(reinterpret_cast<char*>(&temp), Isize);
      Info.write(reinterpret_cast<char*>(&temp), Isize);
      Info.seekg(Isize, std::ios::cur);
      Info.seekp(Isize, std::ios::cur);
    }
    return;
  }

  void insert(int pos0, int posI, info target)
  {
    if (sum == 0)
    {

    }
    int size;
    info temp;
    Info.seekg(pos0, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&size), sizeof(int));
    Info.seekg(sizeof(int) + size * Isize - Isize, std::ios::beg);
    Info.seekp(sizeof(int) + size * Isize, std::ios::beg);
    for (int i = 1; i < size - (posI - pos0 + sizeof(int)) / Isize; i++)
    {
      Info.read(reinterpret_cast<char*>(&temp), Isize);
      Info.write(reinterpret_cast<char*>(&temp), Isize);
      Info.seekg(-Isize, std::ios::cur);
      Info.seekp(-Isize, std::ios::cur);
    }
    Info.write(reinterpret_cast<char*>(&target), Isize);
    return;
  }
};


#endif