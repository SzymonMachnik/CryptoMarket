#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;

struct MemoryStruct {
  vector<char> memory; 
};

size_t writeMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  MemoryStruct* mem = (MemoryStruct*)userp;

  mem->memory.insert(mem->memory.end(), (char*)contents, (char*)contents + realsize);

  return realsize;
}

string returnMemory(const MemoryStruct& mem) {
  return string(mem.memory.begin(), mem.memory.end());
}

int main() {

  CURL *curl;
  CURLcode result;
  MemoryStruct chunk;

  curl = curl_easy_init();

  if (curl == NULL) {
    cout << "ERROR: HTTP request failed" << endl;
    return 1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
  
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk); 


  result = curl_easy_perform(curl);

  cout << returnMemory(chunk) << endl;

  if (result != CURLE_OK) {
    cout << "ERROR: " << curl_easy_strerror(result) << endl;
    return 2;
  }

  chunk.memory.push_back('\0');

  curl_easy_cleanup(curl);
  cout << "Finished succesful" << endl;
  
  return 0;
}
