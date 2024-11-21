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
