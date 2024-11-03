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
  // vector<string> cryptoNameVector;
  // vector<string> cryptoApiIdVector;
  // cryptoNameVector = crypto.getCryptoNameVector();
  // cryptoApiIdVector = crypto.getCryptoApiIdVector();

  // for (auto n : cryptoNameVector) {
  //   cout << n << endl;
  // }
  // cout << endl;
  // for (auto a : cryptoApiIdVector) {
  //   cout << a << endl;
  // }
  //

  //TEST3 FORMATSTRINGRECEIVEDFROMREQUEST() /////////
  Memory memory;
  string data = "{\"avalanche-2\":{\"usd\":24.09},\"binancecoin\":{\"usd\":568.55},\"bitcoin\":{\"usd\":69412},\"cardano\":{\"usd\":0.352065},\"dogecoin\":{\"usd\":0.158325},\"ethereum\":{\"usd\":2487.54},\"ripple\":{\"usd\":0.509382},\"solana\":{\"usd\":163.76},\"the-open-network\":{\"usd\":4.86},\"tron\":{\"usd\":0.166085}}";
  memory.formatStringReceivedFromRequestToMap(data);
  cout << data << endl;
  
  return 0;
}