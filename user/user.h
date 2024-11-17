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

    void setUserDataDuringLogin();
    void generateUserWallet();

    void setBalanceInDb();
    bool doesLoginExistInDb(string login);
    void insertUserIntoDb();


    double returnPriceOfCrypto(string crypto);
        

  public:
    User();

    bool getUserLoginStatus();
    void loginUser();
    void registerUser();

    void deposit(int moneyInCents);
    int getBalanceInCents();

    void buyCrypto();
    void buyCrypto(string crypto);

};

#endif