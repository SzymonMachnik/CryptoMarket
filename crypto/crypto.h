// crypto.h

#ifndef CRYPTO_H
#define CRYPTO_H

#include <sqlite3.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <mutex>

using namespace std;


class Crypto {
  private:
    map<string, string> cryptoApiIdAndNameMap;

    vector<string> cryptoNameVector;
    vector<string> cryptoApiIdVector;
    string PATHTOCRYPTONAMES = "crypto/listOfCryptoNames.txt";
    string PATHTOCRYPTOAPIIDS = "crypto/listOfCryptoApiIds.txt";

    mutex db_mutex;

  public:
    int numberOfCrypto = 0;
    Crypto();
    Crypto(string PATHTOCRYPTONAMES, string PATHTOCRYPTOAPIIDS);

    map<string, string> getCryptoApiIdAndNameMap();
    vector<string> getCryptoNameVector();
    vector<string> getCryptoApiIdVector();

    string getCryptoName(int cryptoId);
    string getCryptoPrice(int cryptoId);

    string getCryptoPrice(string cryptoName);
    int getCryptoId(string cryptoName);

    
};

#endif