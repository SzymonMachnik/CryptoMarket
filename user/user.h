// user.h

#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <sstream>
#include <ctime>
#include "../crypto/crypto.h"

using namespace std;


class User {
  private:
    // User data
    int userId;
    string password;
    string login; 
    bool isUserLoged;
    string firstName;
    string lastName;

    // Balance and limits
    int balanceInCents;
    int balanceAboveWhichUserCantDepositInCents;

    // Manage user data
    void setUserLogin();
    void setUserPassword();
    void setFirstName();
    void setLastName();
    void setUserId();
    void setUserDataDuringLogin();

    // User db
    void setBalanceInDb();
    bool doesLoginExistInDb(string login);
    void insertUserIntoDb();

    // Wallet
    void generateUserWallet();
    bool doesCryptoExistInWallet(string cryptoName);
    void insertCryptoToWallet(string cryptoName, double cryptoAmount, double cryptoPrice, int valueCent);
    void setCryptoToWallet(string cryptoName, double cryptoAmount);
    void walletUpdatePrice();
    double getAmountOfCryptoInWallet(int cryptoId);
    void decreaseCryptoAmountInWallet(string cryptoName, double cryptoAmount);
    void deleteCryptoFromWallet(int cryptoId);

    // Crypto buy-sell setters
    string setCryptoNameToBuy();
    double setCryptoAmountToBuy();
    string setCryptoNameToSell();
    double setCryptoAmountToSell();

    // Transactions list
    void generateUserTransactionsList();
    string getCurrentDate();
    void insertTransactionToTransactionsList(string cryptoName, double cryptoAmount, double cryptoPrice, int valueCent, string type);

    // Crypto info
    // string getCryptoPrice(string crypto);
    // int getCryptoId(string cryptoName);
        
  public:
    // Constructor
    User();

    // Registration and login
    bool getUserLoginStatus();
    void loginUser();
    void loginUser(string tempLogin, string tempPassword);
    void registerUser();

    // Balance
    void deposit(int moneyInCents);
    int getBalanceInCents();

    // Crypto
    void buyCrypto();
    void sellCrypto();

    // Displaying
    void displayWallet();
    void displayTransactionsList();

};

#endif