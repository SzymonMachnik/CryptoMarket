// crypto.cpp

#include "crypto.h"
#include <iostream>
#include <fstream>

using namespace std;

Crypto::Crypto() {
  ifstream fileNames;
  fileNames.open(PATHTOCRYPTONAMES);
  ifstream fileApiIds;
  fileApiIds.open(PATHTOCRYPTOAPIIDS);

  if (fileNames.is_open() == false) {
    cout << "Can't open crypto names file" << endl;
  } else if (fileApiIds.is_open() == false)  {
    cout << "Can't open crypto api ids file" << endl;
  } else {
    string lineName;
    string lineApiId;

    while(fileNames && fileApiIds) {
      getline(fileNames, lineName);
      cryptoNames.insert(lineName);

      getline(fileApiIds, lineApiId);
      cryptoApiIds.insert(lineApiId);
      cryptoIdsByNamesMap[lineName] = lineApiId;
    }
    fileNames.close();
    fileApiIds.close();
  }
}

Crypto::Crypto(string PATHTOCRYPTONAMES, string PATHTOCRYPTOAPIIDS) {
  this->PATHTOCRYPTONAMES = PATHTOCRYPTONAMES;
  this->PATHTOCRYPTOAPIIDS = PATHTOCRYPTOAPIIDS;

  ifstream fileNames;
  fileNames.open(PATHTOCRYPTONAMES);
  ifstream fileApiIds;
  fileApiIds.open(PATHTOCRYPTOAPIIDS);

  if (fileNames.is_open() == false) {
    cout << "Can't open crypto names file" << endl;
  } else if (fileApiIds.is_open() == false)  {
    cout << "Can't open crypto api ids file" << endl;
  } else {
    string lineName;
    string lineApiId;

    while(fileNames && fileApiIds) {
      getline(fileNames, lineName);
      cryptoNames.insert(lineName);

      getline(fileApiIds, lineApiId);
      cryptoApiIds.insert(lineApiId);
      cryptoIdsByNamesMap[lineName] = lineApiId;
    }
    fileNames.close();
    fileApiIds.close();
  }
}

string Crypto::returnCryptoNamesAsString() {
  string res;

  for (auto& element : cryptoNames) {
    res += element + "\n";
  }
  
  return res;
}

vector<string> Crypto::returnCryptoNamesAsVector() {
  vector<string> res;

  for (auto& element : cryptoNames) {
    res.push_back(element);
  }
  
  return res;
}

map<string, string> Crypto::returnCryptoNamesAndIdsAsMap() {
  return cryptoIdsByNamesMap;
}


bool Crypto::include(string coin) {
  if (cryptoNames.find(coin) != cryptoNames.end()) return true;
  return false;
}