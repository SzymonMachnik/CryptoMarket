#include <iostream>
#include <curl/curl.h>
#include <string>

#include "memory/memory.h"
#include "crypto/crypto.h"
using namespace std;

int main() {
  // Init
  CURL* curl;
  CURLcode res;
  Memory memory;
  Crypto crypto;
  
  curl = curl_easy_init();
  if (!curl) {
    cout << "ERROR: Failed to initialize CURL." << endl;
    return 1;
  }


  // Wywołanie funkcji wykonującej zapytanie i zapisującej wynik
  memory.makeRequestAndWriteMemory(Memory::WriteCallback, curl, res);

  // Wyświetlenie wyniku
  cout << "Odpowiedź API:\n" << memory.chunk << endl;

  // Czyszczenie zasobów CURL
  curl_easy_cleanup(curl);


  return 0;
}