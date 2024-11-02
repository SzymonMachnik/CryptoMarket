#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <map>
#include "../memory/memory.h"
#include "../crypto/crypto.h"
#include "../user/user.h"
#include <thread>
#include <chrono>
using namespace std;



int main() {
  Crypto crypto("../crypto/listOfCryptoNames.txt", "../crypto/listOfCryptoApiIds.txt");


  //TEST1 CRYPTO.INCLUDE()//////////////////////////////////////////////////////////////////
  // cout << "\nTEST1 CRYPTO.INCLUDE()" << endl;

  // cout << "1. EXPECTED TRUE  RETURNED: " << crypto.include("bitcoin") << endl;
  // cout << "2. EXPECTED TRUE  RETURNED: " << crypto.include("avalanche") << endl;
  // cout << "3. EXPECTED TRUE  RETURNED: " << crypto.include("dogecoin") << endl;
  // cout << "4. EXPECTED FALSE \t RETURNED: " << crypto.include("solan") << endl;


  //TEST2 CRYPTO cryptoApiIdsByNamesMap/////////////////////////////////////////////////////
  // cout << "\nTEST2 CRYPTO cryptoApiIdsByNamesMap" << endl;

  // vector<string> cryptoNames = crypto.returnCryptoNamesAsVector();
  // map<string, string> cryptoNamesAndIdsMap = crypto.returnCryptoNamesAndIdsAsMap();
  // for (string s : cryptoNames) {
  //   cout << cryptoNamesAndIdsMap[s] << endl;
  // }

  //TEST3 CORECTNESS NAMES IN listOfCryptoNames.txt AND APIIDS IN listOfCryptoApiIds////////
  // cout << "\nTEST3 CORECTNESS NAMES IN listOfCryptoNames.txt" << endl;
  // CURL *curl;
  // CURLcode result;
  // Memory chunk;

  // curl = curl_easy_init();

  // if (curl == NULL) {
  //   cout << "ERROR: HTTP request failed" << endl;
  //   return 1;
  // }
  
  // //GET DATA
  // vector<string> vectorOfCryptoNames = crypto.returnCryptoNamesAsVector();
  // map<string, string> cryptoNamesAndIdsMap = crypto.returnCryptoNamesAndIdsAsMap();

  // for (string cryptoName : vectorOfCryptoNames) {

  //   string address = "https://api.coingecko.com/api/v3/simple/price?ids=";
  //   address += cryptoNamesAndIdsMap[cryptoName];
  //   address += "&vs_currencies=usd";

  //   curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
    
  //   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  //   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  //   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Memory::writeMemory);
  //   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk); 

  //   result = curl_easy_perform(curl);

  //   //CHECK CORRECTNESS OF DATA
  //   if (result != CURLE_OK) {
  //     cout << "ERROR: " << curl_easy_strerror(result) << endl;
  //     return 2;
  //   }

  //   std::this_thread::sleep_for(std::chrono::seconds(15));
  // }


  // //PRINT DATA
  // cout << chunk.returnMemoryAsString() << endl;


  //TEST3 MEMORY formatStringGotFromRequest()///////////////////////////////////////////////////// 
  // Memory memory;

  // string data = "{\"bitcoin\":{\"usd\":65969}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  // data = "{\"zzzzzz\":{\"xxx\":99.999}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  // data = "{\"aaaaaa\":{\"xxx\":00.000}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  // data = "{\"xyz\":{\"usd\":123.123}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  // data = "{\"a123z\":{\"usd\":1aaax}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  // data = "{\"avalanche-2\":{\"usd\":1aaax}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  // data = "{\"the-open-network\":{\"usd\":1aaax}}";
  // memory.formatStringGotFromRequest(data);
  // cout << data << endl;

  //TEST4 USER registerUser() loginUser()///////////////////////////////////////////////////// 

  User user;

  user.registerUser();
  user.loginUser();

  return 0;
}