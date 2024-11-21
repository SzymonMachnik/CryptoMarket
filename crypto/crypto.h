// crypto.h

#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;


class Crypto {
  private:
    map<string, string> cryptoApiIdAndNameMap;

    vector<string> cryptoNameVector;
    vector<string> cryptoApiIdVector;
    string PATHTOCRYPTONAMES = "crypto/listOfCryptoNames.txt";
    string PATHTOCRYPTOAPIIDS = "crypto/listOfCryptoApiIds.txt";

  public:
    int numberOfCrypto = 0;
    Crypto();
    Crypto(string PATHTOCRYPTONAMES, string PATHTOCRYPTOAPIIDS);

    map<string, string> getCryptoApiIdAndNameMap();
    vector<string> getCryptoNameVector();
    vector<string> getCryptoApiIdVector();
};

#endif