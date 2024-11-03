// memory.cpp

#include <iostream>

#include "memory.h"

using namespace std;


void Memory::makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                      vector<string> apiId, CURL* curl, CURLcode &result) {
  // Create url request
  string url = "https://api.coingecko.com/api/v3/simple/price?ids=";
  for (string a : apiId) {
    url += a;
    url += ",";
  }
  url.pop_back();
  url += "&vs_currencies=usd";

  // Curl setup
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this); 

  // Make a request
  result = curl_easy_perform(curl);

  // Check correctness of request
  if (result != CURLE_OK) {
    cout << "ERROR: " << curl_easy_strerror(result) << endl;
  }
}

size_t Memory::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  Memory* mem = static_cast<Memory*>(userp);
  string chunk((char*)contents, realsize);

  mem->formatStringReceivedFromRequestToMap(chunk);
  
  return realsize;
}

void Memory::formatStringReceivedFromRequestToMap(string &data) {

  Crypto crypto;
  map<string, string> mapIdAndNameCrypto;
  mapIdAndNameCrypto = crypto.getCryptoApiIdAndNameMap(); 

  for (int i = 0; i < data.size(); i++) {
    if (data[i] == '"') {
      string cryptoApiId;
      int j = i + 1;
      while (j < data.size() && data[j] != '"') {
        cryptoApiId += data[j];
        j++;
      }

      string cryptoName = mapIdAndNameCrypto[cryptoApiId];

      string cryptoPrice;
      i = j + 1;
      if (i + 7 < data.size()) {
        i += 7;
        j = i + 1;  // Moving to price
        while (j < data.size() && data[j] != '}') {
          cryptoPrice += data[j];
          j++;
        }
        i = j + 1;
      } else {
        break;
      }
      float f_cryptoPrice = stof(cryptoPrice);
      mapOfCryptosNameAndPrice[cryptoName] = f_cryptoPrice;
    }
  }
}

map<string, float> Memory::getMapOfCryptosIdAndPrice() {
  return mapOfCryptosNameAndPrice;
}

void Memory::printMapOfCryptosIdAndPrice() {
  // cout << "Map size: " << mapOfCryptosNameAndPrice.size() << endl;
  
  for (auto it : mapOfCryptosNameAndPrice) {
    cout << it.first << " : " << it.second << endl;
  }
}