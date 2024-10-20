#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include "memory/memory.h"
#include "crypto/crypto.h"

using namespace std;

void makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                Memory &chunk, string cryptoApiId, CURL* curl, CURLcode &result) {
  //GET DATA
  curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
  
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
  
  //ASK USER FOR CRYPTO's name
  //Check if the name of this crypto is correct
  //Get crypto's api id
  //make an request
  //print result
  makeRequestAndWriteMemory(Memory::writeMemory, chunk, "ethereum", curl, result);


  //PRINT DATA
  cout << chunk.returnMemoryAsString() << endl;


  //CLOSING OPERATIONS 
  curl_easy_cleanup(curl);
  cout << "Finished succesful" << endl;
  
  return 0;
}