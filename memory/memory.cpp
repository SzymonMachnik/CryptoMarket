// memory.cpp

#include <iostream>

#include "memory.h"

using namespace std;

size_t Memory::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  Memory* mem = static_cast<Memory*>(userp);
  string chunk((char*)contents, realsize);

  mem->formatStringReceivedFromApiRequestAndSaveIntoDb(chunk);
  
  return realsize;
}

void Memory::makeRequestAndWriteMemory(const vector<string> &apiId, CURL* curl, CURLcode &result) {
  // Create url request
  string url = "https://api.coingecko.com/api/v3/simple/price?ids=";
  for (string a : apiId) {
    url += a;
    url += ",";
  }
  url.pop_back();
  url += "&vs_currencies=usd";

  // Curl setup
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); 

  // Make a request
  result = curl_easy_perform(curl);

  // Check correctness of request
  if (result != CURLE_OK) {
    cout << "ERROR: " << curl_easy_strerror(result) << endl;
  }

}

void Memory::updateCryptoPriceIntoDB(string name, double price) {
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect with databse
  if (sqlite3_open("sqlite/database.db", &db)) {
    cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Create sql request
  ostringstream sql;
  sql << "UPDATE crypto_price SET price = " << price
      << " WHERE name = '" << name << "';";

  // Make a request
  if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "TESTBłąd podczas wykonywania zapytania: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  // Close
  sqlite3_close(db);
}

void Memory::formatStringReceivedFromApiRequestAndSaveIntoDb(string &data) {

  Crypto crypto;
  map<string, string> mapIdAndNameCrypto;
  mapIdAndNameCrypto = crypto.getCryptoApiIdAndNameMap(); 

  for (int i = 0; i < data.size(); i++) {
    if (data[i] == '"') {
      string cryptoApiId;
      int j = i + 1;
      while (j < data.size() && data[j] != '"') {
        cryptoApiId += data[j];
        j++;
      }

      string cryptoName = mapIdAndNameCrypto[cryptoApiId];

      string cryptoPrice;
      i = j + 1;
      if (i + 7 < data.size()) {
        i += 7;
        j = i + 1;  // Moving to price
        while (j < data.size() && data[j] != '}') {
          cryptoPrice += data[j];
          j++;
        }
        i = j + 1;
      } else {
        break;
      }
      double d_cryptoPrice = stod(cryptoPrice);

      updateCryptoPriceIntoDB(cryptoName, d_cryptoPrice);
    }
  }
}

void Memory::printCryptoNameAndPriceDb() {
  // Init
  sqlite3 *db;
  char *errMsg = nullptr;

  // Connect to database
  if (sqlite3_open("sqlite/database.db", &db) != SQLITE_OK) {
    cerr << "Can not open database: " << sqlite3_errmsg(db) << endl;
    return;
  }
  
  // Create a sql request
  const char *sql = "SELECT name, price FROM crypto_price ORDER BY price DESC";

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
  if (sqlite3_exec(db, sql, callback, &result, &errMsg) != SQLITE_OK) {
    cerr << "Error in making db request: " << errMsg << endl;
    sqlite3_free(errMsg);
  }

  cout << result << endl;

  // Close
  sqlite3_close(db);
}