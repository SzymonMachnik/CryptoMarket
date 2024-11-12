// memory.h

#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <curl/curl.h>
#include "../crypto/crypto.h"
#include <vector>
#include <string>
#include <map>
#include <sqlite3.h>
#include <sstream>

using namespace std;


class Memory {
  private:

  public:
    void makeRequestAndWriteMemory(const vector<string> apiId, CURL* curl, CURLcode &result);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void formatStringReceivedFromApiRequestAndSaveIntooDb(string &data);
    void updateCryptoPriceIntoDB(string name, double price);
    void printCryptoNameAndPriceDb();
};

#endif