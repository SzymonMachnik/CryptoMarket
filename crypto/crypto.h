// crypto.h

#ifndef CRYPTO_H
#define CRYPTO_H
#include <iostream>
#include <set>
#include <string>

using namespace std;
class Crypto {
  private:
    set<string> cryptoNames; 
    string PATHTOCRYPTONAMES = "crypto/listOfCryptoNames.txt";

  public:
    Crypto();
    Crypto(string PATHTOCRYPTONAMES);
    string returnCryptoNames();
    bool include(string coin);
};

#endif