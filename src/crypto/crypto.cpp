// crypto.cpp

#include <iostream>
#include <fstream>

#include "crypto.h"

using namespace std;


Crypto::Crypto() {
  ifstream fileNames;
  fileNames.open(PATHTOCRYPTONAMES);
  ifstream fileApiIds;
  fileApiIds.open(PATHTOCRYPTOAPIIDS);

  if (fileNames.is_open() == false) {
    cout << "ERROR: Can't open crypto names file" << endl;
  } else if (fileApiIds.is_open() == false)  {
    cout << "ERROR: Can't open crypto api ids file" << endl;
  } else {
    string lineName;
    string lineApiId;

    while (getline(fileNames, lineName) && getline(fileApiIds, lineApiId)) {

      cryptoNameVector.push_back(lineName);
      cryptoApiIdVector.push_back(lineApiId);

      cryptoApiIdAndNameMap[lineApiId] = lineName;
    }
    fileNames.close();
    fileApiIds.close();
  }

  numberOfCrypto = cryptoNameVector.size();
}

Crypto::Crypto(string PATHTOCRYPTONAMES, string PATHTOCRYPTOAPIIDS) {
  this->PATHTOCRYPTONAMES = PATHTOCRYPTONAMES;
  this->PATHTOCRYPTOAPIIDS = PATHTOCRYPTOAPIIDS;

  ifstream fileNames;
  fileNames.open(PATHTOCRYPTONAMES);
  ifstream fileApiIds;
  fileApiIds.open(PATHTOCRYPTOAPIIDS);

  if (fileNames.is_open() == false) {
    cout << "ERROR: Can't open crypto names file" << endl;
  } else if (fileApiIds.is_open() == false)  {
    cout << "ERROR: Can't open crypto api ids file" << endl;
  } else {
    string lineName;
    string lineApiId;

    while (getline(fileNames, lineName) && getline(fileApiIds, lineApiId)) {
      
      cryptoNameVector.push_back(lineName);
      cryptoApiIdVector.push_back(lineApiId);

      cryptoApiIdAndNameMap[lineApiId] = lineName;
    }
    fileNames.close();
    fileApiIds.close();
  }

  numberOfCrypto = cryptoNameVector.size();
}

map<string, string> Crypto::getCryptoApiIdAndNameMap() {
  return cryptoApiIdAndNameMap;
}

vector<string> Crypto::getCryptoNameVector() {
  return cryptoNameVector;
}

vector<string> Crypto::getCryptoApiIdVector() {
  return cryptoApiIdVector;
}

string Crypto::getCryptoName(int cryptoId) {

  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return "-1";
  }

  // Create a sql request
  ostringstream sql;
  sql << "SELECT name FROM crypto_price WHERE crypto_id = '" << cryptoId << "';";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (size_t i = 0; i < argc; i++) {
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

  return result;
}

string Crypto::getCryptoPrice(int cryptoId) {
  
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return "-1";
  }

  // Create a sql request
  ostringstream sql;
  sql << "SELECT price FROM crypto_price WHERE crypto_id = '" << cryptoId << "';";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (size_t i = 0; i < argc; i++) {
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

  return result;
}

string Crypto::getCryptoPrice(string cryptoName) {
  
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;
  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Nie można otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return "-1";
  }

  // Create a sql request
  ostringstream sql;
  sql << "SELECT price FROM crypto_price WHERE name = '" << cryptoName << "';";

  // Callback as lambda
  string cryptoPrice = "";
  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    if (argc > 0 && argv[0]) {
      *static_cast<string *>(data) = argv[0]; 
    }
    return 0;
  };
  // Make a sql request
  if (sqlite3_exec(db, sql.str().c_str(), callback, &cryptoPrice, &errMsg) != SQLITE_OK) {
    cerr << "Błąd w zapytaniu SQL: " << errMsg << endl;
    sqlite3_free(errMsg);
    sqlite3_close(db);
    return "-1";
  }

  // Close
  sqlite3_close(db);
  // cout << cryptoPrice << endl;

  return cryptoPrice;
}

int Crypto::getCryptoId(string cryptoName) {
  
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return -1;
  }

  // Create a sql request
  ostringstream sql;
  sql << "SELECT crypto_id FROM crypto_price WHERE name = '" << cryptoName << "';";

  auto callback = [](void *data, int argc, char **argv, char **azColName) -> int {
    string *result = static_cast<string *>(data);
    for (size_t i = 0; i < argc; i++) {
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

  return stoi(result);
}