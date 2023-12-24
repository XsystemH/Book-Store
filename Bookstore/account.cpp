//
// Created by 13199 on 2023/12/21.
//
#include "account.h"

bool User::Check(int need) {
  if (Privilege < need) return false;
  else return true;
}
void User::su(std::string UID, std::string PW) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty())
  {
    // todo error
    return;
  }
  User ready(*result.begin());
  if (PW == ready.Password) {
    stack.push_back(ready);
  }
  else {
    // todo error
    return;
  }
}
void User::su(std::string UID) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty())
  {
    // todo error
    return;
  }
  if (Check(result.begin()->pos)) {
    User ready(*result.begin());
    stack.push_back(ready);
  }
  else {
    // todo error
    return;
  }
}
void User::lo(std::string UID) {
  if (stack.empty()) {
    // todo error
    return;
  }
  stack.pop_back(); // Pop out the last element in the login stack.
}
void User::rg(std::string UID, std::string PW, std::string UN) {
  // add user in the file
  // need privilege: 0
  if (!accounts.find(UID).empty()) {
    // todo error
    return;
  }
  Account_info new_user;
  strcpy(new_user.index, UID.c_str());
  strcpy(new_user.value, UN.c_str());
  strcpy(new_user.Password, PW.c_str());
  new_user.pos = 1;
  accounts.insert(new_user);
}
void User::pw(std::string UID, std::string CP, std::string NP) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    // todo error
    return;
  }
  if (std::string(result.begin()->Password) != CP) {
    // todo wrong password
    return;
  }
  accounts.deleteI(*result.begin());
  strcpy(result.begin()->Password, NP.c_str());
  accounts.insert(*result.begin());
}
void User::pw(std::string UID, std::string NP) {
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    // todo error
    return;
  }
  if (!Check(7)) {
    // todo privilege 7 is required
    return;
  }
  accounts.deleteI(*result.begin());
  strcpy(result.begin()->Password, NP.c_str());
  accounts.insert(*result.begin());
}
void User::ua(std::string UID, std::string PW, int PV, std::string UN) {
  if (!Check(PV)) {
    // todo higher privilege is required
    return;
  }
  if (!accounts.find(UID).empty()) {
    // todo user exists already
    return;
  }
  Account_info new_user;
  strcpy(new_user.index, UID.c_str());
  strcpy(new_user.value, UN.c_str());
  strcpy(new_user.Password, PW.c_str());
  new_user.pos = PV;
  accounts.insert(new_user);
}
void User::de(std::string UID) {
  if (!Check(7)) {
    // todo need privilege 7
    return;
  }
  std::vector<Account_info> result = accounts.find(UID);
  if (result.empty()) {
    // todo null
    return;
  }
  for (User t : stack) {
    if (t.UserID == UID) {
      // todo in stack
      return;
    }
  }
  accounts.deleteI(*result.begin());
}