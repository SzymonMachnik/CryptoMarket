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

    string firstName;
    string lastName;

    int balanceInCents;
    int balanceAboveWhichUserCantDepositInCents;

    string setFirstName();
    string setLastName();

  public:
    User();

    bool getUserLoginStatus();
    void loginUser();
    void registerUser();

    void saveData();

    void deposit(int moneyInCents);
    int getBalanceInCents();
};

#endif