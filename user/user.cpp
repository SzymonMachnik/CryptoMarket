// user.cpp

#include <iostream>
#include <fstream>

#include "user.h"

using namespace std;

User::User() {
  this->isUserLoged = false;
  this->balanceAboveWhichUserCantDepositInCents = 500000000; // 5 000 000 . 00
}

bool User::getUserLoginStatus() {
  return isUserLoged;
}

void User::insertUserIntoDb() {
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect with databse
  if (sqlite3_open("sqlite/database.db", &db)) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create sql request
  ostringstream sql;
  sql << "INSERT INTO users (login, password, first_name, last_name)"
      << " VALUES ('" << this->login << "', '" << this->password << "', '" << this->firstName << "', '" << this->lastName << "');";

  // Make a request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);
}

void User::setUserDataDuringLogin() {
  sqlite3* db;
  sqlite3_open("sqlite/database.db", &db);

  // Create sql request
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

void User::loginUser() {
  string tempLogin;
  cout << "Enter login to your account: ";
  cin >> tempLogin;

  sqlite3 *db;
  char *errMsg = nullptr;

  ostringstream sql;
  sql << "SELECT password FROM users WHERE login = '" << tempLogin << "';";

  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *passwordInDb = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *passwordInDb += string(argv[i]);
    }
    
    return 0;
  };
  
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

bool User::doesLoginExistInDb(string login) {
  sqlite3 *db;
  char *errMsg = nullptr;

  ostringstream sql;
  sql << "SELECT user_id FROM users WHERE login = '" << login << "';";

  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return false;
  }

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *result += string(azColName[i]) + ": " + (argv[i] ? argv[i] : "NULL") + "\t";
    }
    *result += "\n";
    return 0;
  };
  
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  sqlite3_close(db);

  if (result.empty()) return false;
  else return true;
}

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

void User::setUserId() {
  if (login.size() == 0) {
    cout << "ERROR setUserId: Login to get userId doesn't exist" << endl;
    return;
  }

  sqlite3 *db;
  char *errMsg = nullptr;

  ostringstream sql;
  sql << "SELECT user_id FROM users WHERE login = '" << this->login << "';";

  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (int i = 0; i < argc; i++) {
      *result += string(argv[i]);
    }
    
    return 0;
  };
  
  string result;
  if (sqlite3_exec(db, sql.str().c_str(), callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  sqlite3_close(db);

  this->userId = result[0] - '0';

  cout << userId << endl;
  
}

void User::registerUser() {

  setUserLogin();
  setUserPassword();

  setFirstName();
  setLastName();

  this->isUserLoged = true;
  this->balanceInCents = 0;

  insertUserIntoDb();
  cout << "Welcome " << firstName << " " << lastName << "!" << endl;
  
  setUserId();
}

void User::setBalanceInDb() {
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect with databse
  if (sqlite3_open("sqlite/database.db", &db)) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create sql request
  ostringstream sql;
  sql << "UPDATE users SET balance_cent = " << this->balanceInCents <<
         " WHERE user_id = " << this->userId << ";";

  // Make a request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "Błąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);  
}

void User::deposit(int moneyToDepositInCents) {
  if (balanceInCents > balanceAboveWhichUserCantDepositInCents) {
    // do nothing
  } else if (balanceInCents + moneyToDepositInCents > balanceAboveWhichUserCantDepositInCents) {
    balanceInCents = balanceAboveWhichUserCantDepositInCents;
    setBalanceInDb();
  } else {
    balanceInCents += moneyToDepositInCents;
    setBalanceInDb();
  }
}

int User::getBalanceInCents() {
  return balanceInCents;
}



double User::returnPriceOfCrypto(string crypto) {
  sqlite3 *db;
  char *errMsg = nullptr;
  ostringstream sql;
  sql << "SELECT price FROM crypto_price WHERE name = '" << crypto << "';";

  // Otwieramy bazę danych
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie można otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return -1.0; // Zwracamy wartość sygnalizującą błąd
  }

  // Callback jako lambda
  double cryptoPrice = 0.0;
  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    if (argc > 0 && argv[0]) {
      *static_cast<double *>(data) = stod(argv[0]);
    }
    return 0;
  };

  // Wykonujemy zapytanie SQL
  if (sqlite3_exec(db, sql.str().c_str(), callback, &cryptoPrice, &errMsg) != SQLITE_OK) {
    cerr << "Błąd w zapytaniu SQL: " << errMsg << endl;
    sqlite3_free(errMsg);
    sqlite3_close(db);
    return -1.0;
  }

  // Zamykamy bazę danych
  sqlite3_close(db);

  return cryptoPrice;
}

void User::buyCrypto() {
  string crypto;
  cout << "Enter crypto which you are like to buy: ";
  cin >> crypto;
  cin.ignore(1000, '\n');

  double price = returnPriceOfCrypto(crypto);
  if (price == 0.0) return;

  string amount;
  cout << "Enter how much " << crypto << " you want to buy." << endl;
  cin >> amount;
  cin.ignore(1000, '\n'); 
  double d_amount = stod(amount);
  

  cout << price * d_amount << endl;
  int amountToSpendInCent = price * d_amount * 100 + 1;
  cout << "Amount to spend (cents): " << amountToSpendInCent << endl;
  cout << "Balance (cents): " << balanceInCents << endl;
  
  if (amountToSpendInCent > this->balanceInCents) {
    cout << "You can't afford it." << endl;
    return;
  }

  balanceInCents -= amountToSpendInCent;
  setBalanceInDb();
  cout << "Bought succesful" << endl;

}

void User::buyCrypto(string crypto) {
  // string crypto;
  // cout << "Enter crypto which you are like to buy: ";
  // cin >> crypto;
  // cin.ignore(1000, '\n');

  double price = returnPriceOfCrypto(crypto);

  cout << crypto << " price: " << (price > 0.0 ? to_string(price) : "doesn't exist") << endl;
}