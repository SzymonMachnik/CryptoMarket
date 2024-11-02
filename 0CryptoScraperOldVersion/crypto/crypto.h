// crypto.h

#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <map>

using namespace std;


class Crypto {
  private: 
    set<string> cryptoNames;
    set<string> cryptoApiIds;
    map<string, string> cryptoIdsByNamesMap;
    string PATHTOCRYPTONAMES = "crypto/listOfCryptoNames.txt";
    string PATHTOCRYPTOAPIIDS = "crypto/listOfCryptoApiIds.txt";

  public:
    Crypto();
    Crypto(string PATHTOCRYPTONAMES, string PATHTOCRYPTOAPIIDS);
    string returnCryptoNamesAsString();
    vector<string> returnCryptoNamesAsVector();
    bool include(string coin);
    map<string, string> returnCryptoNamesAndIdsAsMap();
    string returnCryptoIdByName(string name);
};

#endif