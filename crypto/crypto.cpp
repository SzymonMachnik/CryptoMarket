// crypto.cpp

#include "crypto.h"
#include <iostream>
#include <fstream>

using namespace std;

Crypto::Crypto() {
  ifstream file;
  file.open(PATHTOCRYPTONAMES);

  if (file.is_open() == false) {
    cout << "Can't open" << endl;
  } else {
    string line;

    while(file) {
      getline(file, line);
      cryptoNames.insert(line);
    }
    file.close();
  }
}

Crypto::Crypto(string PATHTOCRYPTONAMES) {
  this->PATHTOCRYPTONAMES = PATHTOCRYPTONAMES;
  ifstream file;
  file.open(PATHTOCRYPTONAMES);

  if (file.is_open() == false) {
    cout << "Can't open" << endl;
  } else {
    string line;

    while(file) {
      getline(file, line);
      cryptoNames.insert(line);
    }
    file.close();
  }
}

string Crypto::returnCryptoNames() {
  string res;

  for (auto& element : cryptoNames) {
    res += element + "\n";
  }
  
  return res;
}