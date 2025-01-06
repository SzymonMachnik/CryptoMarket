#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;


int main() {
  const string PATHTOCRYPTONAMES = "../src/crypto/listOfCryptoNames.txt";
  const string PATHTOCRYPTOAPIIDS = "../src/crypto/listOfCryptoApiIds.txt";

  ifstream fileNames;
  fileNames.open(PATHTOCRYPTONAMES);
  ifstream fileApiIds;
  fileApiIds.open(PATHTOCRYPTOAPIIDS);

  vector<string> cryptoNameVector;
  vector<string> cryptoApiIdVector;

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

    }
    fileNames.close();
    fileApiIds.close();
  }

  
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("./database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return 1;
  }

  // Wyczyszczenie tabeli
  const char *clearTableSQL = "DELETE FROM crypto_price;";
  if (sqlite3_exec(db, clearTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
      std::cerr << "Błąd podczas czyszczenia tabeli: " << errMsg << std::endl;
      sqlite3_free(errMsg);
  }

  // Zresetowanie autoincrement
  const char *resetAutoIncrementSQL = "DELETE FROM sqlite_sequence WHERE name = 'crypto_price';";
  if (sqlite3_exec(db, resetAutoIncrementSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
      std::cerr << "Błąd podczas resetowania AI: " << errMsg << std::endl;
      sqlite3_free(errMsg);
  }

  for (size_t i = 0; i < cryptoNameVector.size(); i++) {

    //cout << cryptoNameVector[i] << cryptoApiIdVector[i] << endl;
    // Create a sql request
    ostringstream sql;
    sql << "INSERT INTO crypto_price (name, api)"
        << "VALUES ('" << cryptoNameVector[i] << "', '" << cryptoApiIdVector[i] << "');";

    // Wykonanie zapytania SQL
    if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Błąd wykonania zapytania SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
  }
  // Close
  sqlite3_close(db);

  return 0;
}