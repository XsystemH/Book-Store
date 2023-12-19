#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <utility>
#include <vector>

const size_t Max = 150;

template<class node, class info>
class blockchain
{

private:

  int Nsum;
  int head;
  size_t Nsize;
  size_t Isize;
  std::string NODE;
  std::string INFO;
  std::fstream Node;
  std::fstream Info;

public:
// fstream f;
// f.open("dsa");
// if(!f.good()){
//   f.open("dsa", std::ios::out);
//   f.close();
//   f.open("dsa");
// }
  blockchain(std::string Nname, std::string Iname)
  {
    Nsum = 0;
    NODE = Nname;
    INFO = Iname;
    Nsize = sizeof(node);
    Isize = sizeof(info);

    Node.open(NODE);
    if (!Node.good()) // 打开文件异常
    {
      Node.open(NODE, std::ios::out);
      Node.close();
      Node.open(NODE); // 创建文件

      head = -1;
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&head), sizeof(int)); // 写入初始化head指针 指向-1(means null)
      Node.seekp(sizeof(int), std::ios::beg);
      Node.write(reinterpret_cast<char*>(&Nsum), sizeof(int));
    }
    else {
      Node.seekg(0, std::ios::beg);
      Node.read(reinterpret_cast<char*>(&head), sizeof(int)); // 读入已有head
      Node.seekg(sizeof(int), std::ios::beg);
      Node.read(reinterpret_cast<char*>(&Nsum), sizeof(int)); // 读入已有Nsum
    }
    Node.close();

    Info.open(INFO);
    if (!Info.good())
    {
      Info.open(INFO, std::ios::out | std::ios::binary);
      Info.close();
      Info.open(INFO);
      Info.seekp(0, std::ios::beg);
      int temp = 0;
      Info.write(reinterpret_cast<char*>(&temp), sizeof(int));
    }
    Info.close();

    // 当你使用close函数关闭文件时，会自动刷新缓冲区中的数据，
    // 确保数据被写入到文件中。而不关闭文件的话，
    // 缓冲区中的数据可能还未被写入文件，这样可能会导致数据丢失。
  }

  ~blockchain() = default;

  std::vector<std::pair<int, int>> findN(std::string index) // 感觉没问题
  {
    Node.open(NODE);
    std::vector<std::pair<int, int>> poss;
    node tpr;
    int hp;
    int posN;
    Node.seekg(0, std::ios::beg);
    std::cout << "now at: " << Node.tellg() << std::endl;
    Node.read(reinterpret_cast<char*>(&hp), sizeof(int));
    Node.seekg(2 * sizeof(int) + hp * Nsize, std::ios::beg);
    hp = head;
    Node.read(reinterpret_cast<char*>(&tpr), Nsize);
    bool fir = true;
    do
    {
      std::string maxn_str = tpr.maxn;
      std::string minn_str = tpr.minn;
      posN = Node.tellg();
      posN -= Nsize;
      if (index > maxn_str)
      {
        break;
      }
      else if (minn_str <= index && index < maxn_str)
      {
        poss.push_back(std::make_pair(posN, tpr.pos));
        break;
      }
      else if (fir && index < minn_str)
      {
        poss.push_back(std::make_pair(posN, tpr.pos));
        break;
      }
      else if (index == maxn_str)
      {
        poss.push_back(std::make_pair(posN, tpr.pos));
      }
//      else if (index < minn_str)
//      {
//        break;
//      }
      fir = false;
      if (tpr.next != -1) {
        Node.seekg(tpr.next, std::ios::beg);
        Node.read(reinterpret_cast<char *>(&tpr), Nsize);
      }
    }while (tpr.next != -1);
    return poss;
  }

  // 对于所有的index 找到对应value的index
  bool findI(std::vector<std::pair<int, int>> &poss, info &target, /*提供的信息*/  int &pos0, int &posI, int &posN /*返回的信息*/)
  {
    Info.open(INFO);
    int size;
    for (int i = 0; i < poss.size(); i++) // 多个块中可能包含所对应的内容
    {
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      posN = poss[i].first;
      pos0 = poss[i].second;

      info block[Max];
//      Info.seekg(sizeof(int), std::ios::cur);
//      std::cout << "now at: " << Info.tellg() << std::endl;
      Info.read(reinterpret_cast<char*>(&block), size * Isize);

      int l = 0, r = size - 1;
      int mid;
      while (l <= r)
      {
        mid = (l + r) / 2;
        std::string bstr = block[mid].index;
        std::string tstr = target.index;
        if (bstr < tstr || bstr == tstr && block[mid].value <= target.value)
        {
          l = mid + 1;
          posI = mid + 1;
          if (bstr == tstr && block[mid].value == target.value)
          {
            posI = mid;
            Info.close();
            return true;
          }
        }
        else if (bstr > tstr || block[mid].value > target.value)
        {
          r = mid - 1;
          posI = mid;
        }
      } // while
    } // for i
    Info.close();
    return false;
  }

  std::vector<int> findI(std::vector<std::pair<int, int>> &poss, std::string index)
  {
    Info.open(INFO);
    std::vector<int> vals;
    int size;
    info block[Max];
    for (int i = 0; i < poss.size(); i++)
    {
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
//      Info.seekg(sizeof(int), std::ios::cur);
      Info.read(reinterpret_cast<char*>(&block), Isize * size);
      for(int j = 0; j < size; j++)
      {
        if(std::string(block[j].index) == index)
        {
          vals.push_back(block[j].value);
        }
      }
    }
    Info.close();
    return vals;
  }

  void deleteI(int pos0, int posI)
  {
    Info.open(INFO);
    int size;
    info block[Max];
    Info.seekg(pos0, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&size), sizeof(int));
    Info.seekp(pos0, std::ios::beg);
    Info.write(reinterpret_cast<char*>(size - 1), sizeof(int));
    // 读取 并 修改块的大小
    int temp = (posI - pos0 + sizeof(int)) / Isize;
    Info.seekg(pos0 + sizeof(int), std::ios::beg);
    Info.read(reinterpret_cast<char*>(&block), size * Isize);
    // 读取完整数组
    Info.seekp(pos0 + sizeof(int), std::ios::beg);
    Info.write(reinterpret_cast<char*>(&block), temp * Isize);
    // 写入被Delete前面的部分
//    Info.seekp(temp * Isize, std::ios::cur);
    Info.write(reinterpret_cast<char*>(block + temp), (size - temp - 1) * Isize);
    // 写入后半 相当于删除操作
    Info.close();
  }

  // main直接调用 先找到需要删除的位置 再调用删除函数
  void deleteI(info target)
  {
    int pos0, posI, posN;
    std::vector<std::pair<int, int>> poss;
    poss = findN(target.index);
    bool found = findI(poss, target, pos0, posI, posN);
    if (found) deleteI(pos0, posI);
  }

  void insert(int pos0, int posI, int posN, info target)
  {
    Node.open(NODE);
    Info.open(INFO);
    // moved
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

    int back = size - posI;
    info block[Max];
    Info.seekg(pos0 + sizeof(int), std::ios::beg);
    Info.read(reinterpret_cast<char*>(&block), Isize * size);
    for (int i = size; i > posI; i--) {
      block[i] = block[i - 1];
    }
    block[posI] = target;
    size++;
    Info.seekp(pos0, std::ios::beg);
    Info.write(reinterpret_cast<char*>(&size), sizeof(int));
    Info.write(reinterpret_cast<char*>(&block), Isize * size);

    if (back == 0)
    {
      Node.seekg(posN, std::ios::beg);
      Node.seekp(posN, std::ios::beg);
      node old;
      Node.read(reinterpret_cast<char*>(&old), Nsize);
      strcpy(old.maxn, target.index);
      Node.write(reinterpret_cast<char*>(&old), Nsize);
      // 更新节点最大范围
    }
    Node.close();
    Info.close();
  }

  void insert(info &target)
  {
    if (Nsum == 0)
    {
      Node.open(NODE);
      Info.open(INFO);
      head = 0; // 0 based
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&head), sizeof(int)); // head 指向第一个Node
      Nsum = 1;
      Node.seekp(sizeof(int), std::ios::beg);
      Node.write(reinterpret_cast<char*>(&Nsum), sizeof(int)); // Nsum = 1
      Info.seekp(0, std::ios::beg);
      int tempnum = 1;
      Info.write(reinterpret_cast<char*>(&tempnum), sizeof(int));
      Info.write(reinterpret_cast<char*>(&target), Isize); // 在第一个节点的sum之后写第一个内容

      node newnode;
      newnode.pos = 0;
      newnode.next = -1;
      newnode.last = -1;
      strcpy(newnode.maxn, target.index);
      strcpy(newnode.minn, target.index);
      Node.seekp(2*sizeof(int), std::ios::beg);
      Node.write(reinterpret_cast<char*>(&newnode), Nsize);

      Node.close();
      Info.close();
      return;
    } // 创建新的node节点
    int pos0, posI, posN;
    std::vector<std::pair<int, int>> poss;
    poss = findN(target.index);
    findI(poss, target, pos0, posI, posN);
    insert(pos0, posI, posN, target);
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
    strcpy(oldnode.maxn, temp.index);
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
        strcpy(newnode.minn, temp.index);
      } // 记录newnode范围
    }
    strcpy(newnode.maxn, temp.index);
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