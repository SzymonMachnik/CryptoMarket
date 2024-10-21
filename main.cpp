#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <bits/stdc++.h> //to transform string to lower case
#include "memory/memory.h"
#include "crypto/crypto.h"

using namespace std;

void makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                Memory &chunk, string cryptoApiId, CURL* curl, CURLcode &result) {
  //CREATE AND REQUEST ADDRESS
  string address = "https://api.coingecko.com/api/v3/simple/price?ids=" + cryptoApiId + "&vs_currencies=usd";

  //GET DATA
  curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
  
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk); 

  result = curl_easy_perform(curl);

  //CHECK CORRECTNESS OF DATA
  if (result != CURLE_OK) {
    cout << "ERROR: " << curl_easy_strerror(result) << endl;
  }
}


int main() {
  //INIT
  CURL *curl;
  CURLcode result;
  Memory chunk;
  Crypto crypto;

  curl = curl_easy_init();

  if (curl == NULL) {
    cout << "ERROR: HTTP request failed" << endl;
    return 1;
  }
   

  //Ask user for crypto's name
  string cryptoName = "";
  cout << "Enter the name of the crypto you wnat to know the price of.\nNAME: ";
  cin >> cryptoName;
  transform(cryptoName.begin(), cryptoName.end(), cryptoName.begin(), ::tolower);

  //Check if the name of this crypto is correct
  if (crypto.include(cryptoName)) {
    //Get crypto's api id
    string cyrptoApiId = crypto.returnCryptoIdByName(cryptoName);

    //Make a request
    makeRequestAndWriteMemory(Memory::writeMemory, chunk, cyrptoApiId, curl, result);

    //Print data
    cout << chunk.returnMemoryAsString() << endl;

  } else {
    //Return inaccurance if crypto name is wrong
    cout << "Inaccurance: Wrong crypto name" << endl;
    cout << "=========================================" << endl;
    cout << "List of available names of cryptos below:" << endl;
    cout << crypto.returnCryptoNamesAsString();
    cout << "=========================================" << endl;
  }


  //CLOSING OPERATIONS 
  curl_easy_cleanup(curl);
  cout << "Finished succesful" << endl;
  
  return 0;
}