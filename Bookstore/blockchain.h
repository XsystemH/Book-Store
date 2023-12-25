#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <utility>
#include <vector>

const size_t Max = 512;

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
  bool emp = true;

public:

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

      head = 0;
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&head), sizeof(int)); // 写入初始化head指针 指向-1(means null)
      Node.seekp(sizeof(int), std::ios::beg);
      Node.write(reinterpret_cast<char*>(&Nsum), sizeof(int));
    }
    else {
      emp = false;
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

  bool empty() {
    return emp;
  }

  std::vector<int> findA() // find all the value
  {
    Node.open(NODE);
    std::vector<std::pair<int, int>> poss;
    node tpr;
    int hp;
    int posN;
    Node.seekg(0, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&hp), sizeof(int));
    Node.seekg(2 * sizeof(int) + hp * Nsize, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&tpr), Nsize);
    do
    {
      std::string maxn_str = tpr.maxn;
      std::string minn_str = tpr.minn;
      posN = Node.tellg();
      posN -= Nsize;
      poss.push_back(std::make_pair(posN, tpr.pos));
      if (tpr.next != -1) {
        Node.seekg(tpr.next, std::ios::beg);
        Node.read(reinterpret_cast<char *>(&tpr), Nsize);
      }
    }while (tpr.next != -1);
    Node.close();

    std::vector<int> vals;
    int size;
    info block[Max];
    Info.open(INFO);
    for (int i = 0; i < poss.size(); i++)
    {
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      Info.read(reinterpret_cast<char*>(&block), Isize * size);
      for(int j = 0; j < size; j++)
      {
        vals.push_back(block[j].pos);
      }
    }
    Info.close();
    return vals;
  }

  std::vector<std::pair<int, int>> findN(std::string index) // 感觉没问题
  {
    Node.open(NODE);
    std::vector<std::pair<int, int>> poss;
    node tpr;
    int hp;
    int posN;
    Node.seekg(0, std::ios::beg);
//    std::cout << "now at: " << Node.tellg() << std::endl;
    Node.read(reinterpret_cast<char*>(&hp), sizeof(int));
    Node.seekg(2 * sizeof(int) + hp * Nsize, std::ios::beg);
//    hp = head;
    Node.read(reinterpret_cast<char*>(&tpr), Nsize);
    bool fir = true;
    bool more = false;
    do
    {
      more = false;
      std::string maxn_str = tpr.maxn;
      std::string minn_str = tpr.minn;
      posN = Node.tellg();
      posN -= Nsize;
      if (index > maxn_str)
      {
        if (tpr.next == -1) {
          fir = false;
          poss.push_back(std::make_pair(posN, tpr.pos));
        }
      }
      else if (minn_str <= index && index < maxn_str)
      {
        fir = false;
        poss.push_back(std::make_pair(posN, tpr.pos));
        break;
      }
      else if (fir && index < minn_str)
      {
        fir = false;
        poss.push_back(std::make_pair(posN, tpr.pos));
        break;
      }
      else if (index == maxn_str)
      {
        fir = false;
        poss.push_back(std::make_pair(posN, tpr.pos));
      }
//      else if (index < minn_str)
//      {
//        break;
//      }
      if (tpr.next != -1) {
        Node.seekg(tpr.next, std::ios::beg);
        Node.read(reinterpret_cast<char *>(&tpr), Nsize);
        more = true;
      }
    }while (tpr.next != -1 || more);
    Node.close();
    return poss;
  }

  // 对于所有的index 找到对应value的index
  bool findI(std::vector<std::pair<int, int>> &poss, info &target, /*提供的信息*/  int &pos0, int &posI, int &posN /*返回的信息*/)
  {
    Info.open(INFO);
    int size;
    posI = 0;
    for (int i = 0; i < poss.size(); i++) // 多个块中可能包含所对应的内容
    {
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      posN = poss[i].first;
      pos0 = poss[i].second;
      if (size == 0) continue;

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
        std::string bval = block[mid].value;
        std::string tval = target.value;
        if (tstr < bstr || (tstr == bstr && tval <= bval)) {
          posI = mid;
          r = mid - 1;
          if (tstr == bstr && tval == bval) {
            Info.close();
            return true;
          }
        }
        else {
          posI = mid + 1;
          l = mid + 1;
        }
      } // while
      if (r != size - 1 && size >= 1) {
        Info.close();
        return false;
      }
    } // for i
    Info.close();
    return false;
  }

  std::vector<int> findI(std::vector<std::pair<int, int>> &poss, std::string index)
  {
    std::vector<int> vals;
    int size;
    info block[Max];
    for (int i = 0; i < poss.size(); i++)
    {
      Info.open(INFO);
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
//      Info.seekg(sizeof(int), std::ios::cur);
      Info.read(reinterpret_cast<char*>(&block), Isize * size);
      for(int j = 0; j < size; j++)
      {
        if(std::string(block[j].index) == index)
        {
//          std::string bstr = block[j].index;
          vals.push_back(block[j].pos);
        }
      }
//      vals.push_back(-1);
      Info.close();
    }
    return vals;
  }

  void deleteI(int pos0, int posI, int posN)
  {
    Info.open(INFO);
    int size;
    info block[Max];
    Info.seekg(pos0, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&size), sizeof(int));
    // 读取块的大小
    int temp = posI;
    Info.seekg(pos0 + sizeof(int), std::ios::beg);
    Info.read(reinterpret_cast<char*>(&block), size * Isize);
    // 读取完整数组
    size--;
    for (int i = posI; i < size; i++) {
      block[i] = block[i + 1];
    }
    Info.seekp(pos0, std::ios::beg);
    Info.write(reinterpret_cast<char*>(&size), sizeof(int));
    Info.write(reinterpret_cast<char*>(&block), size * Isize);
    if (posI == size) {
      Node.open(NODE);
      node changenode;
      Node.seekg(posN, std::ios::beg);
      Node.read(reinterpret_cast<char*>(&changenode), Nsize);
      strcpy(changenode.maxn, block[size - 1].index);
      Node.seekp(posN, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&changenode), Nsize);
      Node.close();
    }
    if (posI == 0) {
      Node.open(NODE);
      Node.seekg(posN, std::ios::beg);
      node changenode;
      Node.read(reinterpret_cast<char*>(&changenode), Nsize);
      if (size != 0) strcpy(changenode.minn, block[0].index);
      else {
        strcpy(changenode.maxn, "");
        strcpy(changenode.minn, "");
      }
      Node.seekp(posN, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&changenode), Nsize);
      Node.close();
    }
    Info.close();
  }

  // main直接调用 先找到需要删除的位置 再调用删除函数
  void deleteI(info target)
  {
    int pos0, posI, posN;
    std::vector<std::pair<int, int>> poss;
    poss = findN(target.index);
    bool found = findI(poss, target, pos0, posI, posN);
    if (found) deleteI(pos0, posI, posN);
  }

  void insert(int pos0, int posI, int posN, info target)
  {
    emp = false;
    Node.open(NODE);
    Info.open(INFO);
    // moved
    int size;
    info temp;
    Info.seekg(pos0, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&size), sizeof(int));

    if (size == Max)
    {
      Node.close();
      Info.close();
      devide(pos0, posI, posN, size, target);
      return;
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

    node old;
    Node.seekg(posN, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&old), Nsize);
    strcpy(old.minn, block[0].index);
    strcpy(old.maxn, block[size - 1].index);
    Node.seekp(posN, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&old), Nsize);
    // 更新节点范围
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
//      newnode.last = -1; didn't use
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

  void devide(int pos0, int posI, int posN, int size, info target)
  {
    Node.open(NODE);
    Info.open(INFO);
    node oldnode;
    node newnode;
    info temp;
    Node.seekg(posN, std::ios::beg);
//    std::cout << "now at: " << Node.tellg() << std::endl;
    Node.read(reinterpret_cast<char*>(&oldnode), Nsize);

    info block[Max];
    info newblock[Max];
    int size1 = Max / 2;
    int size2 = Max / 2;
    Info.seekg(pos0 + sizeof(int), std::ios::beg);
    Info.read(reinterpret_cast<char*>(&block), Max * Isize);

    for (int i = 0; i < Max / 2; i++) {
      newblock[i] = block[i + Max / 2];
    }

    if (posI < Max / 2) {
      for (int i = size1; i > posI; i--) {
        block[i] = block[i - 1];
      }
      block[posI] = target;
      size1++;
    }
    else {
      posI -= Max / 2;
      for (int i = size1; i > posI; i--) {
        newblock[i] = newblock[i - 1];
      }
      newblock[posI] = target;
      size2++;
    }
    Info.seekp(pos0, std::ios::beg);
    Info.write(reinterpret_cast<char*>(&size1), sizeof(int));
    Info.write(reinterpret_cast<char*>(&block), size1 * Isize);
    int iend = (sizeof(int) + Isize * Max) * Nsum;
    int nend = 2 * sizeof(int) + Nsize * Nsum;
    Nsum++;
    Info.seekp(iend, std::ios::beg);
    Info.write(reinterpret_cast<char*>(&size2), sizeof(int));
    Info.write(reinterpret_cast<char*>(&newblock), size2 * Isize);
    strcpy(oldnode.minn, block[0].index);
    strcpy(oldnode.maxn, block[size1 - 1].index);
    strcpy(newnode.minn, newblock[0].index);
    strcpy(newnode.maxn, newblock[size2 - 1].index);
    newnode.pos = iend;
    newnode.next = oldnode.next;
    oldnode.next = nend;
    Node.seekp(sizeof(int), std::ios::beg);
    Node.write(reinterpret_cast<char*>(&Nsum), sizeof(int));
    Node.seekp(posN, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&oldnode), Nsize);
    Node.seekp(nend, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&newnode), Nsize);

    Node.close();
    Info.close();
  }

  std::vector<info> find(std::string index) {
    std::vector<std::pair<int, int>> poss = findN(index);
    std::vector<info> vals;
    int size;
    info block[Max];
    for (int i = 0; i < poss.size(); i++)
    {
      Info.open(INFO);
      Info.seekg(poss[i].second, std::ios::beg);
      Info.read(reinterpret_cast<char*>(&size), sizeof(int));
      Info.read(reinterpret_cast<char*>(&block), Isize * size);
      for(int j = 0; j < size; j++)
      {
        if(std::string(block[j].index) == index)
        {
          vals.push_back(block[j]);
        }
      }
      Info.close();
    }
    return vals;
  }
};


#endif