#include <iostream>
#include <curl/curl.h>
#include <string>
#include <map>
#include "../memory/memory.h"
#include "../crypto/crypto.h"
using namespace std;


int main() {
  Crypto crypto("../crypto/listOfCryptoNames.txt", "../crypto/listOfCryptoApiIds.txt");


  //TEST1 GETCRYPTONAMEANDIDMAP///////////////////
  // map<string, string> map;
  // map = crypto.getCryptoApiIdAndNameMap();
  // for (auto it : map) {
  //   cout << it.first << " : " << it.second << endl;
  // }

  //TEST2 GETCRYPTONAMEVECTOR & GETCRYPTOAPIIDVECTOR///////////////////
  vector<string> cryptoNameVector;
  vector<string> cryptoApiIdVector;
  cryptoNameVector = crypto.getCryptoNameVector();
  cryptoApiIdVector = crypto.getCryptoApiIdVector();

  for (auto n : cryptoNameVector) {
    cout << n << endl;
  }
  cout << endl;
  for (auto a : cryptoApiIdVector) {
    cout << a << endl;
  }
  //

  return 0;
}