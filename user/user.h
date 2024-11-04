// user.h

#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


class User {
  private:
    string password;
    string login; 
    bool isUserLoged;

    int balanceInCents;
    int balanceAboveWhichUserCantDepositInCents;

  public:
    User();

    bool getUserLoginStatus();
    void loginUser();
    void registerUser();

    void deposit(int moneyInCents);
    int getBalanceInCents();
};

#endif