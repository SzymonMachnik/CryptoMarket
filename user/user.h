// user.h

#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <sstream>

using namespace std;


class User {
  private:

    int userId;

    string password;
    string login; 
    bool isUserLoged;

    string firstName;
    string lastName;

    int balanceInCents;
    int balanceAboveWhichUserCantDepositInCents;

    void setUserLogin();
    void setUserPassword();


    void setFirstName();
    void setLastName();

    void setUserId();

  public:
    User();

    bool doesLoginExistInDb(string login);

    bool getUserLoginStatus();
    void loginUser();
    void registerUser();

    void saveData();

    void deposit(int moneyInCents);
    int getBalanceInCents();
};

#endif