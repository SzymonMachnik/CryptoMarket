// memory.cpp

#include <iostream>

#include "memory.h"

using namespace std;


void Memory::makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                      vector<string> apiId, CURL* curl, CURLcode &result) {
  // Tworzenie adresu URL dla zapytania
  string url = "https://api.coingecko.com/api/v3/simple/price?ids=";
  for (string a : apiId) {
    url += a;
    url += ",";
  }
  url.pop_back();
  url += "&vs_currencies=usd";
  
  // Czyszczenie odpowiedzi na nową próbę
  chunk.clear();

  // Ustawienia CURL dla pobrania danych z adresu URL
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemory);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk); 

  // Wykonanie zapytania
  result = curl_easy_perform(curl);

  // Sprawdzanie poprawności odpowiedzi
  if (result != CURLE_OK) {
    cout << "ERROR: " << curl_easy_strerror(result) << endl;
  }
}

size_t Memory::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t total_size = size * nmemb;
  static_cast<string*>(userp)->append(static_cast<char*>(contents), total_size);
  return total_size;
}