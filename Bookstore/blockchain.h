#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <utility>
#include <vector>

const size_t Max = 300;

template<class node, class info>
class blockchain
{

private:

  int Nsum;
  int Isum;
  size_t Nsize;
  size_t Isize;
  std::string NODE;
  std::string INFO;
  std::fstream Node;
  std::fstream Info;

public:

  blockchain(std::string Nname, std::string Iname)
  {
    Nsum = 0;
    Isum = 0;
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

  std::vector<std::pair<int, int>> findN(std::string &index)
  {
    std::vector<std::pair<int, int>> poss;
    node tpr;
    Node.seekg(0, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&tpr), Nsize);
    int posN;
    while (tpr.next != -1)
    {
      posN = Node.tellg();
      if (index > tpr.maxn)
      {
        continue;
      }
      else if (tpr.minn <= index && index < tpr.maxn)
      {
        poss.push_back(std::make_pair(posN, tpr.pos));
        break;
      }
      else if (index == tpr.maxn)
      {
        poss.push_back(std::make_pair(posN, tpr.pos));
      }
      else if (index < tpr.minn)
      {
        break;
      }
      Node.seekg(tpr.next);
      Node.read(reinterpret_cast<char*>(&tpr), Nsize);
    }
    return poss;
  }

  bool findI(std::vector<std::pair<int, int>> &poss, info &target, int &pos0, int &posI, int &posN)
  {
    int size;
    info val;
    for(int i = 0; i < poss.size(); i++)
    {
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      posN = poss[i].first;
      pos0 = poss[i].second;
      for(int j = 0; j <= size; j++)
      {
        Info.seekg(Isize, std::ios::cur);
        Info.read(reinterpret_cast<char*>(&val), Isize);
        posI = Info.tellg();
        if(val.index == target.index && val.value == target.value)
        {
          return true;
        }
        if(val.index == target.index && val.value > target.value)
        {
          return false;
        }
      }
    }
    return false;
  }

  std::vector<int> findI(std::vector<std::pair<int, int>> &poss, std::string index)
  {
    std::vector<int> vals;
    int size;
    info val;
    for(int i = 0; i < poss.size(); i++)
    {
      Info.seekg(poss[i].second, std::ios::beg);
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

  void insert(int pos0, int posI, int posN, info target)
  {
    if (Nsum == 0)
    {
      node head;
      head.maxn = target.index;
      // head.minn = target.index;
      // 最小设置为0？
      head.pos = 0;
      head.next = -1;
      head.last = -1;
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&head), Nsize);
      Info.seekp(0, std::ios::beg);
      Info.write(reinterpret_cast<char*>((int)1), sizeof(int));
      Info.seekp(sizeof(int), std::ios::cur);
      Info.write(reinterpret_cast<char*>(&target), Isize);
      Nsum++;
      return;
    } // 创建新的node节点
    int size;
    info temp;
    Info.seekg(pos0, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&size), sizeof(int));

    if (size == Max)
    {
      if (devide(pos0, posI, posN, size, target))
        return;
      size = Max / 2;
    }

    Info.seekg(sizeof(int) + size * Isize - Isize, std::ios::beg);
    Info.seekp(sizeof(int) + size * Isize, std::ios::beg);
    int back = size - (posI - pos0 - sizeof(int)) / Isize;
    for (int i = 1; i <= back; i++)
    {
      Info.read(reinterpret_cast<char*>(&temp), Isize);
      Info.write(reinterpret_cast<char*>(&temp), Isize);
      Info.seekg(-Isize, std::ios::cur);
      Info.seekp(-Isize, std::ios::cur);
    }
    Info.write(reinterpret_cast<char*>(&target), Isize);
    Isum++;
    if (back == 0)
    {
      Node.seekg(posN, std::ios::beg);
      Node.seekp(posN, std::ios::beg);
      node old;
      Node.read(reinterpret_cast<char*>(&old), Nsize);
      old.maxn = target.index;
      Node.write(reinterpret_cast<char*>(&old), Nsize);
      // 更新节点最大范围
    }
    return;
  }

  void insert(info target)
  {
    int pos0, posI, posN;
    std::vector<std::pair<int, int>> poss;
    poss = findN(target.index);
    findI(poss, target, pos0, posI, posN);
    insert(pos0, posI, posN, target);
    return;
  }

  bool devide(int pos0, int posI, int posN, int size, info target)
  {
    bool did_insert = false;
    node oldnode;
    node newnode;
    info temp;
    Node.seekg(posN, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&oldnode), Nsize);
    // 找到目标位置
    int topos = sizeof(int) + Nsum * Isize * Max;
    newnode.pos = topos;
    Info.seekg(pos0 + (Max / 2 - 1) * Isize + sizeof(int), std::ios::beg);
    // 修改原node的范围
    Info.read(reinterpret_cast<char*>(&temp), Isize);
    oldnode.maxn = temp.index;
    Info.seekg(Isize, std::ios::cur);
    // 移动到目标位置，复制内容
    Info.seekp(topos, std::ios::beg);
    for(int i = 1; i <= size - Max / 2; i++)
    {
      if(Info.tellg() == posI)
      {
        Info.write(reinterpret_cast<char*>(&target), Isize);
        Info.seekp(Isize, std::ios::cur);
        did_insert = true;
        continue;
      } // 顺便插入

      Info.read(reinterpret_cast<char*>(&temp), Isize);
      Info.write(reinterpret_cast<char*>(&temp), Isize);
      Info.seekg(Isize, std::ios::cur);
      Info.seekp(Isize, std::ios::cur); // 复制

      if(i == 1)
      {
        newnode.minn = temp.index;
      } // 记录newnode范围
    }
    newnode.maxn = temp.index;
    newnode.last = posN;
    newnode.next = oldnode.next;
    Node.seekg(0, std::ios::end);
    oldnode.next = Node.tellg();
    Node.seekp(posN, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&oldnode), Nsize);
    Node.seekp(0, std::ios::end);
    Node.write(reinterpret_cast<char*>(&newnode), Nsize);
    return did_insert;
  }
};


#endif