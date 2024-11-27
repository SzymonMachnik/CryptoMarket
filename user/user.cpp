// user.cpp

#include <iostream>
#include <fstream>

#include "user.h"

using namespace std;

// PUBLIC FUNCTIONS 

//Constructor
User::User() {
  this->isUserLoged = false;
  this->balanceAboveWhichUserCantDepositInCents = 100000000; // 5 000 000 . 00
}


// Registration and login
bool User::getUserLoginStatus() {
  return isUserLoged;
}

void User::loginUser() {
  // Init
  string tempLogin;
  cout << "Enter login to your account: ";
  cin >> tempLogin;

  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request  
  ostringstream sql;
  sql << "SELECT password FROM users WHERE login = '" << tempLogin << "';";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *passwordInDb = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *passwordInDb += string(argv[i]);
    }
    
    return 0;
  };
  
  // Make a sql request
  string passwordInDb;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &passwordInDb, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  sqlite3_close(db);

  if (passwordInDb.size() == 0) {
    cout << "Account with this login doesn't exist." << endl;
  } else {  
    string tempPassword;
    cout << "Enter password to your account: ";
    cin >> tempPassword;
    
    if (tempPassword == passwordInDb) {
      this->login = tempLogin;
      setUserDataDuringLogin();

    } else {
      cout << "Wrong password." << endl;
    }
  }
}

void User::loginUser(string tempLogin, string tempPassword) {
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request  
  ostringstream sql;
  sql << "SELECT password FROM users WHERE login = '" << tempLogin << "';";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *passwordInDb = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *passwordInDb += string(argv[i]);
    }
    
    return 0;
  };
  
  // Make a sql request
  string passwordInDb;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &passwordInDb, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  sqlite3_close(db);

  if (passwordInDb.size() == 0) {
    cout << "Account with this login doesn't exist." << endl;
  } else {      
    if (tempPassword == passwordInDb) {
      this->login = tempLogin;
      setUserDataDuringLogin();

    } else {
      cout << "Wrong password." << endl;
    }
  }
}

void User::registerUser() {

  setUserLogin();
  setUserPassword();

  setFirstName();
  setLastName();

  this->isUserLoged = true;
  this->balanceInCents = 0;

  insertUserIntoDb();
  setUserId();

  generateUserWallet();
  generateUserTransactionsList();

  cout << "Welcome " << firstName << " " << lastName << "!" << endl;
}

int User::registerUser(string tempLogin, string tempPassword, string tempFirstName, string tempLastName) {
  int const USER_OK = 0;
  int errorCode;

  errorCode = setUserLogin(tempLogin);
  if (errorCode != USER_OK) return errorCode;

  errorCode = setUserPassword(tempPassword);
  if (errorCode != USER_OK) return errorCode + 10;

  errorCode = setFirstName(tempFirstName);
  if (errorCode != USER_OK) return errorCode + 100;

  errorCode = setLastName(tempLastName);
  if (errorCode != USER_OK) return errorCode + 1000;

  this->isUserLoged = true;
  this->balanceInCents = 0;

  insertUserIntoDb();
  setUserId();

  generateUserWallet();
  generateUserTransactionsList();

  cout << "Welcome " << firstName << " " << lastName << "!" << endl;
  return 0;
}


// Balance
int User::deposit(int moneyToDepositInCents) {
  if (balanceInCents > balanceAboveWhichUserCantDepositInCents) {
    return 1;
  } else if (moneyToDepositInCents <= 0) {
    return 2;
  } else if (balanceInCents + moneyToDepositInCents > balanceAboveWhichUserCantDepositInCents) {
    balanceInCents = balanceAboveWhichUserCantDepositInCents;
    setBalanceInDb();
    return 3;
  } else {
    balanceInCents += moneyToDepositInCents;
    setBalanceInDb();
  }
  return 0;
}

int User::getBalanceInCents() {
  return balanceInCents;
}


// Crypto
void User::buyCrypto() {
  Crypto crypto;
  string cryptoName = setCryptoNameToBuy();

  string s_cryptoPrice = crypto.getCryptoPrice(cryptoName);
  if (s_cryptoPrice == "") return;
  double cryptoPrice = stod(s_cryptoPrice);

  double cryptoAmount = setCryptoAmountToBuy();

  int moneyToSpendInCent = cryptoPrice * cryptoAmount * 100 + 1;
  cout << "Amount to spend (cents): " << moneyToSpendInCent << endl;
  cout << "Balance (cents): " << balanceInCents << endl;
  
  if (moneyToSpendInCent > this->balanceInCents) {
    cout << "You can't afford it." << endl;
    return;
  }
  
  if (doesCryptoExistInWallet(cryptoName) == false) {
    insertCryptoToWallet(cryptoName, cryptoAmount, cryptoPrice, moneyToSpendInCent);
  } else {
    setCryptoToWallet(cryptoName, cryptoAmount);
    walletUpdatePrice();
  }

  insertTransactionToTransactionsList(cryptoName, cryptoAmount, cryptoPrice, moneyToSpendInCent, "buy");
  
  
  balanceInCents -= moneyToSpendInCent;
  setBalanceInDb();
  cout << "Bought succesful" << endl;
}

int User::buyCrypto(int cryptoId, double cryptoAmountToBuy, int moneyToSpendInCent) {
  Crypto crypto;
  
  if (moneyToSpendInCent > balanceInCents) return 1;
  if (cryptoAmountToBuy == 0) return 2;

  string cryptoName = crypto.getCryptoName(cryptoId);
  double cryptoPrice = stod(crypto.getCryptoPrice(cryptoName));

  if (doesCryptoExistInWallet(cryptoName) == false) {
    insertCryptoToWallet(cryptoName, cryptoAmountToBuy, cryptoPrice, moneyToSpendInCent);
  } else {
    setCryptoToWallet(cryptoName, cryptoAmountToBuy);
  }
  walletUpdatePrice();

  insertTransactionToTransactionsList(cryptoName, cryptoAmountToBuy, cryptoPrice, moneyToSpendInCent, "buy");
  
  
  balanceInCents -= moneyToSpendInCent;
  setBalanceInDb();

  return 0;
}

void User::sellCrypto() {
  Crypto crypto;
  string cryptoName = setCryptoNameToSell();

  if (doesCryptoExistInWallet(cryptoName) == false) {
    cout << "You don't have this cyrpto in your waller" << endl;
    return;
  }

  int cryptoId = crypto.getCryptoId(cryptoName);

  double cryptoPrice = stod(crypto.getCryptoPrice(cryptoName));
  if (cryptoPrice == 0.0) return;

  double cryptoAmountToSell = setCryptoAmountToSell();
  
  double amountInWallet = getAmountOfCryptoInWallet(cryptoId);

  if (cryptoAmountToSell > amountInWallet) {
    cout << "MORE THAN YOU HAVE. YOU CANT SELL IT!" << endl;
    return;
  }

  int moneyToWithdrawInCent = cryptoPrice * cryptoAmountToSell * 100 - 1;

  
  if (cryptoAmountToSell == amountInWallet) {
    deleteCryptoFromWallet(cryptoId);
    cout << "ALL CRYPTO SOLD" << endl;
  } else {
    decreaseCryptoAmountInWallet(cryptoName, cryptoAmountToSell);
    walletUpdatePrice();
    cout << "PART OF CRYPTO SOLD" << endl;
  }

  insertTransactionToTransactionsList(cryptoName, cryptoAmountToSell, cryptoPrice, moneyToWithdrawInCent, "sell");
  
  
  balanceInCents += moneyToWithdrawInCent;
  setBalanceInDb();

}

int User::sellCrypto(int cryptoId, double cryptoAmountToSell, int moneyToWithdrawInCent) {
  if (cryptoAmountToSell <= 0.0 || moneyToWithdrawInCent <= 0) return 1;
  Crypto crypto;
  double cryptoAmountInWallet = getAmountOfCryptoInWallet(cryptoId);
  if (cryptoAmountInWallet == -1. || cryptoAmountToSell > cryptoAmountInWallet) return 2;

  string cryptoName = crypto.getCryptoName(cryptoId);
  double cryptoPrice = stod(crypto.getCryptoPrice(cryptoName));

  if (cryptoAmountToSell == cryptoAmountInWallet) {
    deleteCryptoFromWallet(cryptoId);
  } else {
    decreaseCryptoAmountInWallet(cryptoName, cryptoAmountToSell);
  }
  walletUpdatePrice();
  insertTransactionToTransactionsList(cryptoName, cryptoAmountToSell, cryptoPrice, moneyToWithdrawInCent, "sell");
  
  balanceInCents += moneyToWithdrawInCent;
  setBalanceInDb();

  return 0;
}


// Wallet
void User::displayWallet() {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // IMPORTANT - UPDATE WALLET
  walletUpdatePrice();

  // Create a sql request
  ostringstream sql;
  sql << "SELECT name, amount, price, value_cent FROM wallet_" << this->userId << " ORDER BY price DESC";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
      string *result = static_cast<string *>(data);
      for (int i = 0; i < argc; i++) {
          if (string(azColName[i]) == "value_cent") {            
              double value = stod(argv[i]);
              // tranform to real value (not cents)
              *result += to_string(value / 100)  + "\t";
          } else {
              *result += string(argv[i]) + "\t";
          }
      }
      *result += "\n";
      return 0;
  };
  
  // Make a sql request
  string result;
  cout << "Name \t Amount \t Price \t Value \t" << endl;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  cout << result << endl;

  // Close
  sqlite3_close(db);
}

vector<vector<string>> User::returnAllRecordsFromWallet() {
  // IMPORTANT - UPDATE WALLET
  walletUpdatePrice();
  
  vector<vector<string>> result;
    sqlite3* db;
    if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
      cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
      return result;
    }
    ostringstream sql;
    sql << "SELECT crypto_id, name, value_cent, amount, price FROM wallet_"
        << this->userId << " ORDER BY value_cent DESC";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);


    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        row.push_back(std::to_string(sqlite3_column_int(stmt, 0))); // crypto_id
        row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));        // name
        row.push_back(std::to_string(sqlite3_column_int(stmt, 2))); // value_cent
        row.push_back(std::to_string(sqlite3_column_double(stmt, 3)));        // amount
        row.push_back(std::to_string(sqlite3_column_double(stmt, 4)));     // price
        result.push_back(row);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    //std::reverse(result.begin(), result.end());
    return result;

}

// Transactions
void User::displayTransactionsList() {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }


  // Create a sql request
  ostringstream sql;
  sql << "SELECT transaction_id, cp.name, crypto_amount, crypto_price, value_cent, type FROM transactions_" << this->userId << " t"
      << " LEFT JOIN crypto_price cp"
      << " ON cp.crypto_id = t.crypto_id"
      << " ORDER BY transaction_id DESC";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
      string *result = static_cast<string *>(data);
      for (int i = 0; i < argc; i++) {
        *result += string(argv[i]) + "\t";
      }
      *result += "\n";
      return 0;
  };
  
  // Make a sql request
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  cout << result << endl;

  // Close
  sqlite3_close(db); 
}

vector<vector<string>> User::returnAllTransactions() {
  vector<vector<string>> result;
    sqlite3* db;
    if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
      cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
      return result;
    }
    ostringstream sql;
    sql << "SELECT transaction_id, date, cp.name, crypto_amount, crypto_price, value_cent, type FROM transactions_"
        << this->userId << " t"
        << " JOIN crypto_price cp"
        << " ON t.crypto_id = cp.crypto_id;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);

    // select transaction_id, date, cp.name, crypto_amount, crypto_price, value_cent, type FROM transactions_1 JOIN crypto_price cp ON transactions_1.crypto_id = cp.crypto_id;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        row.push_back(std::to_string(sqlite3_column_int(stmt, 0)));        // transaction_id
        row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))); // date
        row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));        // crypto_id
        row.push_back(std::to_string(sqlite3_column_double(stmt, 3)));     // crypto_amount
        row.push_back(std::to_string(sqlite3_column_double(stmt, 4)));     // crypto_price
        row.push_back(std::to_string(sqlite3_column_int(stmt, 5)));        // value_cent
        row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))); // type
        result.push_back(row);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::reverse(result.begin(), result.end());
    return result;

}


// PRIVATE FUNCTIONS

// Manage user data
void User::setUserLogin() {
  
  string login;

  bool correctLogin = false;
  do {
    cout << "Enter login to your new account: ";
    cin >> login;
    cin.ignore(1000, '\n');

    if (login.find(' ') != string::npos) continue; // checking if login contains space

    // min 10 characters
    // only small letters
    correctLogin = true;
    if (login.size() < 10 || 32 < login.size()) {
      cout << "Login size must to be beteen 10 and 32 characters." << endl;
      correctLogin = false;
    } else {
      for (char c : login) {
        if (!(0 <= c - 'a' && c - 'a' <= 25)) {
          correctLogin = false;
          cout << "Login must contain only letters." << endl;
          break;
        }
      }
    }

    if (correctLogin == true && doesLoginExistInDb(login) == true) {
      correctLogin = false;
      cout << "The login you provided is already in use." << endl; 
    }
    
  } while (correctLogin == false);

  this->login = login;
}

int User::setUserLogin(string tempLogin) {
  bool correctLogin = false;

  // min 10 characters
  // only small letters
  correctLogin = true;
  if (tempLogin.size() < 10 || 32 < tempLogin.size()) {
    cout << "Login size must to be beteen 10 and 32 characters." << endl;
    correctLogin = false;
    return 1;
  } else {
    for (char c : tempLogin) {
      if (!(0 <= c - 'a' && c - 'a' <= 25)) {
        correctLogin = false;
        cout << "Login must contain only letters." << endl;
        return 2;
      }
    }
  }

  if (correctLogin == true && doesLoginExistInDb(tempLogin) == true) {
    correctLogin = false;
    cout << "The login you provided is already in use." << endl; 
    return 3;
  }    

  this->login = tempLogin;
  return 0;
}

void User::setUserPassword() {
  string password;
    
  bool correctPassword = false;
  do {
    cout << "Enter password to your new account: ";
    cin >> password;
    cin.ignore(1000, '\n');
    
    if (password.find(' ') != string::npos) continue; // checking if password contains space

    correctPassword = true;
    int lowerLetterCounter = 0;
    int capitalLetterCounter = 0;
    int digitCounter = 0;
    int specialCharacterCounter = 0;

    int minLowerLetter = 4;
    int minCapitalLetter = 1;
    int minDigit = 2;
    int minSpecialCharacter = 1;

    if (password.size() < 8 || 32 < password.size()) {
      cout << "Password size must to be beteen 8 and 32 characters." << endl;
      correctPassword = false;
    } else {
      for (char c : password) {
        if (0 <= c - 'a' && c - 'a' <= 25) {
          lowerLetterCounter++;
        } else if (0 <= c - 'A' && c - 'A' <= 25) {
          capitalLetterCounter++;
        } else if (0 <= c - '0' && c - '0' <= 9) {
          digitCounter++;
        } else if (33 <= c && c <= 125) {
          specialCharacterCounter++;
        } else {
          correctPassword = false;
        }
      }
    }
    if (correctPassword == true) {
      if (!(lowerLetterCounter >= minLowerLetter
          && capitalLetterCounter >= minCapitalLetter
          && digitCounter >= minDigit
          && specialCharacterCounter >= minSpecialCharacter)) {

        cout << "Password has to contain at least:" << endl;
        cout << minLowerLetter << " small letters" << " Contains: " << lowerLetterCounter << endl;
        cout << minCapitalLetter << " capital letter" << " Contains: " << capitalLetterCounter << endl;
        cout << minDigit << " digits" << " Includes: " << digitCounter << endl;
        cout << minSpecialCharacter << " special character" << " Contains: " << specialCharacterCounter << endl;

        correctPassword = false;
      }
    }
    
  } while (correctPassword == false);

    this->password = password;
}

int User::setUserPassword(string tempPassword) {    
  bool correctPassword = false;

  correctPassword = true;
  int lowerLetterCounter = 0;
  int capitalLetterCounter = 0;
  int digitCounter = 0;
  int specialCharacterCounter = 0;

  int minLowerLetter = 4;
  int minCapitalLetter = 1;
  int minDigit = 2;
  int minSpecialCharacter = 1;

  if (tempPassword.size() < 8 || 32 < tempPassword.size()) {
    cout << "Password size must to be beteen 8 and 32 characters." << endl;
    correctPassword = false;
    return 1;
  } else {
    for (char c : tempPassword) {
      if (0 <= c - 'a' && c - 'a' <= 25) {
        lowerLetterCounter++;
      } else if (0 <= c - 'A' && c - 'A' <= 25) {
        capitalLetterCounter++;
      } else if (0 <= c - '0' && c - '0' <= 9) {
        digitCounter++;
      } else if (33 <= c && c <= 125) {
        specialCharacterCounter++;
      } else {
        correctPassword = false;
        return 2;
      }
    }
  }
  if (correctPassword == true) {
    if (!(lowerLetterCounter >= minLowerLetter
        && capitalLetterCounter >= minCapitalLetter
        && digitCounter >= minDigit
        && specialCharacterCounter >= minSpecialCharacter)) {

      cout << "Password has to contain at least:" << endl;
      cout << minLowerLetter << " small letters" << " Contains: " << lowerLetterCounter << endl;
      cout << minCapitalLetter << " capital letter" << " Contains: " << capitalLetterCounter << endl;
      cout << minDigit << " digits" << " Includes: " << digitCounter << endl;
      cout << minSpecialCharacter << " special character" << " Contains: " << specialCharacterCounter << endl;

      correctPassword = false;
      return 3;
    }
  }
    
  this->password = tempPassword;
  return 0;
}

void User::setFirstName() {
  string firstName;

  do {
    cout << "Enter your first name: ";
    cin >> firstName;
    cin.ignore(1000, '\n');

    if (firstName.find(' ') != string::npos) {
      firstName = "";
      continue;
    }; // checking if name contains space    
  } while (firstName.size() == 0);

  this->firstName = firstName;
}

int User::setFirstName(string tempFirstName) {
  if (tempFirstName.size() == 0) return 1;

  this->firstName = tempFirstName;
  return 0;
}

void User::setLastName() {
  string lastName;

  do {
    cout << "Enter your first name: ";
    cin >> lastName;
    cin.ignore(1000, '\n');

    if (lastName.find(' ') != string::npos) {
      lastName = "";
      continue;
    } // checking if name contains space    
  } while (lastName.size() == 0);

  this->lastName = lastName;
}

int User::setLastName(string tempLastName) {
  if (tempLastName.size() == 0) return 1;

  this->lastName = tempLastName;
  return 0;
}

void User::setUserId() {
  if (login.size() == 0) {
    cout << "ERROR setUserId: Login to get userId doesn't exist" << endl;
    return;
  }

  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request  
  ostringstream sql;
  sql << "SELECT user_id FROM users WHERE login = '" << this->login << "';";


  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *result += string(argv[i]);
    }
    
    return 0;
  };

  // Make a sql request
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);

  this->userId = stoi(result);
}

void User::setUserDataDuringLogin() {
  // Init
  sqlite3* db;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sql;
  sql << "SELECT user_id, login, password, first_name, last_name, balance_cent FROM users WHERE login = '" << this->login << "';";
  sqlite3_stmt* stmt;

    
  if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    cout << "ERROR setUserDataDuringLogin: Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
    return;
  }

  sqlite3_bind_int(stmt, 1, this->userId);

  // Make a request and assing result to user data
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    this->userId = sqlite3_column_int(stmt, 0);
    this->login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    this->password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    this->firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    this->lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    this->balanceInCents = sqlite3_column_int(stmt, 5);
    this->isUserLoged = true;
  } else {
    cout << "ERROR setUserDataDuringLogin: User not found." << endl;
  }

  // Clean up
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}


// User db
void User::setBalanceInDb() {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sql;
  sql << "UPDATE users SET balance_cent = " << this->balanceInCents <<
         " WHERE user_id = " << this->userId << ";";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);  
}

bool User::doesLoginExistInDb(string login) {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return false;
  }
  
  // Create a sql request  
  ostringstream sql;
  sql << "SELECT user_id FROM users WHERE login = '" << login << "';";


  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *result += string(azColName[i]) + ": " + (argv[i] ? argv[i] : "NULL") + "\t";
    }
    *result += "\n";
    return 0;
  };
  
  // Make a sql request
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);

  if (result.empty()) return false;
  else return true;
}

void User::insertUserIntoDb() {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create q sql request
  ostringstream sql;
  sql << "INSERT INTO users (login, password, first_name, last_name)"
      << " VALUES ('" << this->login << "', '" << this->password << "', '" << this->firstName << "', '" << this->lastName << "');";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);
}


// Wallet
void User::generateUserWallet() {
  // Init
  sqlite3* db;
  char* errorMessage = 0;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sqlCreateTable;
  sqlCreateTable << "CREATE TABLE wallet_" << userId << " ("
                    << " crypto_id INTEGER PRIMARY KEY NOT NULL,"
                    << " name TEXT NOT NULL,"
                    << " amount DECIMAL(20, 8) NOT NULL,"
                    << " price DECIMAL(20, 8) NOT NULL,"
                    << " value_cent INTEGER NOT NULL"
                    << " );";

  // Make a sql request
  if (sqlite3_exec(db, sqlCreateTable.str().c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
    cerr << "Błąd podczas tworzenia tabeli: " << errorMessage << endl;
    sqlite3_free(errorMessage);
  }

  // Close
  sqlite3_close(db);
}

bool User::doesCryptoExistInWallet(string cryptoName) {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return false;
  }

  // Create a sql request  
  ostringstream sql;
  sql << "SELECT crypto_id FROM wallet_" << this->userId << " WHERE name = '" << cryptoName << "';";


  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *result += string(argv[i]);
    }
    
    return 0;
  };

  // Make a sql request
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);

  if (result.size() > 0) return true;
  else return false;
}

void User::insertCryptoToWallet(string cryptoName, double cryptoAmount, double cryptoPrice, int valueCent) {
  Crypto crypto;
  int cryptoId = crypto.getCryptoId(cryptoName);
  if (cryptoId == -1) return;

  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sql;
  sql << "INSERT INTO wallet_"<< this->userId << " (crypto_id, name, amount, price, value_cent)"
      << " VALUES ('" << cryptoId << "', '" << cryptoName << "', '" << cryptoAmount << "', '" << cryptoPrice << "', '" << valueCent << "');";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);

}

void User::setCryptoToWallet(string cryptoName, double cryptoAmount) {
  Crypto crypto;
  int cryptoId = crypto.getCryptoId(cryptoName);
  if (cryptoId == -1) return;

  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sql;
  sql << "UPDATE wallet_" << this->userId
      << " SET amount = amount + " << cryptoAmount
      << " WHERE crypto_id = " << cryptoId << ";";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);

}

void User::walletUpdatePrice() {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }
  
  // Create 1st sql request
  ostringstream sql;
  sql << "UPDATE wallet_" << this->userId
      << " SET price = ("
      << "   SELECT cp.price"
      << "   FROM crypto_price cp"
      << "   WHERE cp.crypto_id = wallet_" << this->userId << ".crypto_id"
      << " )";
  // Make 1st sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas aktualizacji kolumny price: " << errMsg << endl;
    sqlite3_free(errMsg);
    return;
  }

  // Create 2nd sql request
  ostringstream sql2;
  sql2 << "UPDATE wallet_" << this->userId
      << " SET value_cent = CAST(price * amount * 100 AS INTEGER)";
  // Make 2nd sql request
  if (sqlite3_exec(db, sql2.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas aktualizacji kolumny value_cent: " << errMsg << endl;
    sqlite3_free(errMsg);
    return;
  }

  // Close
  sqlite3_close(db);
 
}

double User::getAmountOfCryptoInWallet(int cryptoId) {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return -1.;
  }

  // Create a sql request
  ostringstream sql;
  sql << "SELECT amount FROM wallet_" << this->userId << " WHERE crypto_id = '" << cryptoId << "';";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *result += string(argv[i]);
    }
    
    return 0;
  };
  
  // Make a sql reuqest
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);
  if (result == "") return -1.;
  return stod(result);
}

void User::decreaseCryptoAmountInWallet(string cryptoName, double cryptoAmount) {
  Crypto crypto;
  int cryptoId = crypto.getCryptoId(cryptoName);
  if (cryptoId == -1) return;

  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sql;
  sql << "UPDATE wallet_" << this->userId
      << " SET amount = amount - " << cryptoAmount
      << " WHERE crypto_id = " << cryptoId << ";";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);

}

void User::deleteCryptoFromWallet(int cryptoId) {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create q sql request
  ostringstream sql;
  sql << "DELETE FROM wallet_" << this->userId << " WHERE crypto_id = " << cryptoId << ";";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);
}


// Crypto buy-sell setters
string User::setCryptoNameToBuy() {
  string cryptoName;
  cout << "Enter crypto which you are like to buy: ";
  cin >> cryptoName;
  cin.ignore(1000, '\n');

  return cryptoName;
}

double User::setCryptoAmountToBuy() {
  string s_cryptoAmount;
  cout << "Enter how much you want to buy: ";
  cin >> s_cryptoAmount;
  cin.ignore(1000, '\n'); 
  double d_cryptoAmount = stod(s_cryptoAmount);

  return d_cryptoAmount;
}

string User::setCryptoNameToSell() {
  string cryptoName;
  cout << "Enter crypto which you are like to sell: ";
  cin >> cryptoName;
  cin.ignore(1000, '\n');

  return cryptoName;
}

double User::setCryptoAmountToSell() {
  string s_cryptoAmount;
  cout << "Enter how much you want to sell: ";
  cin >> s_cryptoAmount;
  cin.ignore(1000, '\n'); 
  double d_cryptoAmount = stod(s_cryptoAmount);

  return d_cryptoAmount;
}


// Transactions list
void User::generateUserTransactionsList() {
  // Init
  sqlite3* db;
  char* errorMessage = 0;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sqlCreateTable;
  sqlCreateTable << "CREATE TABLE transactions_" << userId << " ("
                    << " transaction_id INTEGER PRIMARY KEY,"
                    << " date DATE NOT NULL,"
                    << " crypto_id INTEGER NOT NULL,"
                    << " crypto_amount DECIMAL(20, 8) NOT NULL,"
                    << " crypto_price DECIMAL(20, 8) NOT NULL,"
                    << " value_cent INTEGER NOT NULL,"
                    << " type TEXT CHECK(type IN ('sell', 'buy')) NOT NULL"
                    << " );";

  // Make a sql request
  if (sqlite3_exec(db, sqlCreateTable.str().c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
    cerr << "Błąd podczas tworzenia tabeli: " << errorMessage << endl;
    sqlite3_free(errorMessage);
  }

  // Close
  sqlite3_close(db);
}

string User::getCurrentDate() {
  time_t now = time(nullptr);

  tm* localTime = localtime(&now);

  ostringstream date;
  date      << 1900 + localTime->tm_year << "-"    // Year
            << 1 + localTime->tm_mon << "-"       // Month
            << localTime->tm_mday;               // Day

  return date.str();
}

void User::insertTransactionToTransactionsList(string cryptoName, double cryptoAmount, double cryptoPrice, int valueCent, string type) {
  Crypto crypto;
  int cryptoId = crypto.getCryptoId(cryptoName);
  if (cryptoId == -1) return;

  string date = getCurrentDate();

  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to databse
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create a sql request
  ostringstream sql;
  sql << "INSERT INTO transactions_"<< this->userId << " (date, crypto_id, crypto_amount, crypto_price, value_cent, type)"
      << " VALUES ('" << date << "', '" << cryptoId << "', '" << cryptoAmount << "', '" << cryptoPrice <<"', '" << valueCent << "', '" << type << "');";

  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);
}
