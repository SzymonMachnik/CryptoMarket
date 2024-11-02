#include <iostream>
#include <curl/curl.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#include "memory/memory.h"
#include "crypto/crypto.h"
using namespace std;


// Init
CURL* curl;
CURLcode res;
Memory memory;
Crypto crypto;


std::atomic<bool> keep_running(true); // Flaga do zatrzymania wątku

void printMessageEvery30Seconds() {
  while (keep_running) {
    std::this_thread::sleep_for(std::chrono::seconds(61)); // Czeka 30 sekund
    // Wywołanie funkcji wykonującej zapytanie i zapisującej wynik
    memory.makeRequestAndWriteMemory(Memory::WriteCallback, crypto.getCryptoApiIdVector(), curl, res);

    // Wyświetlenie wyniku
    cout << "Odpowiedź API:\n" << memory.chunk << endl;
  }
}

int main() {
  // Curl init
  curl = curl_easy_init();
  if (!curl) {
    cout << "ERROR: Failed to initialize CURL." << endl;
    return 1;
  }
  std::thread messageThread(printMessageEvery30Seconds);

  while (true) {
    char input;
    cout << "Input: " << endl;
    cin >> input;
    if (input == 'b') {
      cout << "Crypto boughten" << endl;
    } else if (input == 'q') {
      keep_running = false;
      break;
    } else {
      cout << "Wrong ans" << endl;
    }
  }


  if (messageThread.joinable()) {
    messageThread.join();
  }

  // Czyszczenie zasobów CURL
  curl_easy_cleanup(curl);

  cout << "Finished succesful." << endl;

  return 0;
}