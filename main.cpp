// main.cpp

#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>

#include "memory/memory.h"
#include "crypto/crypto.h"
#include "handleUser/handleUser.h"

using namespace std;


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
  string cryptoName = askUserForCryptoName();

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
    returnInaccuranceCryptoName(crypto);
  }


  //CLOSING OPERATIONS 
  curl_easy_cleanup(curl);
  cout << "Finished succesful" << endl;
  
  return 0;
}