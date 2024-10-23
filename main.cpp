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

  bool ran = true;
  while (ran) {
    
    int decision;

    askForDecision(decision);

    string cryptoName;
    string cryptoApiId;
    switch (decision) {
      case 2:
        ran = false;
        break;
      case 1:
        //Ask user for crypto's name
        cryptoName = askUserForCryptoName();

        //Check if the name of this crypto is correct
        if (crypto.include(cryptoName)) {
          //Get crypto's api id
          cryptoApiId = crypto.returnCryptoIdByName(cryptoName);

          //Make a request
          makeRequestAndWriteMemory(Memory::writeMemory, chunk, cryptoApiId, curl, result);

          //Print data
          cout << chunk.returnMemoryByIndex(Memory::memorySize - 1) << endl;

        } else {
          //Return inaccurance if crypto name is wrong
          returnInaccuranceCryptoName(crypto);
        }
        break;
      default:
        break;
    }

  }



  //CLOSING OPERATIONS 
  curl_easy_cleanup(curl);
  cout << "Finished succesful" << endl;
  
  return 0;
}