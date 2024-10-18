
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include "memory/memory.h"

using namespace std;



int main() {
  //INIT
  CURL *curl;
  CURLcode result;
  Memory chunk;

  curl = curl_easy_init();

  if (curl == NULL) {
    cout << "ERROR: HTTP request failed" << endl;
    return 1;
  }

  //GET DATA
  curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=solana&vs_currencies=usd");
  
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Memory::writeMemory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk); 

  result = curl_easy_perform(curl);

  //CHECK CORRECTNESS OF DATA
  if (result != CURLE_OK) {
    cout << "ERROR: " << curl_easy_strerror(result) << endl;
    return 2;
  }

  //PRINT DATA
  cout << chunk.returnMemoryAsString() << endl;


  //CLOSING OPERATIONS 
  curl_easy_cleanup(curl);
  cout << "Finished succesful" << endl;
  
  return 0;
}