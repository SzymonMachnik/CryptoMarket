// user.h

#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sqlite3.h>
#include <sstream>
#include <ctime>
#include <mutex>
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

    mutex db_mutex;

    // Hashing
    string encrypt(string text);
    string decrypt(string text);

    // Manage user data
    void setUserLogin();
    int setUserLogin(string tempLogin);
    void setUserPassword();
    int setUserPassword(string tempPassword);
    void setFirstName();
    int setFirstName(string tempFirstName);
    void setLastName();
    int setLastName(string tempLastName); 
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
        
  public:
    // Constructor
    User();

    // Registration and login
    bool getUserLoginStatus();
    void loginUser();
    void loginUser(string tempLogin, string tempPassword);
    void registerUser();
    int registerUser(string tempLogin, string tempPassword, string tempFirstName, string tempLastName);

    // Balance
    //void deposit(int moneyInCents);
    int deposit(int moneyToDepositInCents);
    int getBalanceInCents();

    // Crypto
    void buyCrypto();
    int buyCrypto(int cryptoId, double cryptoAmountToBuy, int valueCent);
    void sellCrypto();
    int sellCrypto(int cryptoId, double cryptoAmountToSell, int moneyToWithdrawInCent);
    //void sellCrypto(string cryptoName);

    // Wallet
    void displayWallet();
    vector<vector<string>> returnAllRecordsFromWallet();

    // Transactions
    void displayTransactionsList();
    vector<vector<string>> returnAllTransactions();

};

#endif