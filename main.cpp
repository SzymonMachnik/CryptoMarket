#include <iostream>
#include <curl/curl.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#include "crypto/crypto.h"
#include "memory/memory.h"
using namespace std;


// Init
CURL* curl;
CURLcode res;
Memory memory;
Crypto crypto;


atomic<bool> keep_running(true); // Flag to stop thread

void refreshAndPrintPriceEvery60s() {
  memory.makeRequestAndWriteMemory(Memory::WriteCallback, crypto.getCryptoApiIdVector(), curl, res);
  memory.printMapOfCryptosIdAndPrice();
  while (keep_running) {
    this_thread::sleep_for(chrono::seconds(61)); // Wait 60s for price refresh
    memory.makeRequestAndWriteMemory(Memory::WriteCallback, crypto.getCryptoApiIdVector(), curl, res);
    memory.printMapOfCryptosIdAndPrice();
  } 
}

int main() {
  // Curl init
  curl = curl_easy_init();
  if (!curl) {
    cout << "ERROR: Failed to initialize CURL." << endl;
    return 1;
  }
  thread messageThread(refreshAndPrintPriceEvery60s);


  while (true) {
    char input;
    cout << "Input: " << endl;
    cin >> input;
    if (input == 'b') {
      cout << "Crypto boughten" << endl;
    } else if (input == 'c') {
      memory.printMapOfCryptosIdAndPrice();
    }  else if (input == 'r') {
      memory.makeRequestAndWriteMemory(Memory::WriteCallback, crypto.getCryptoApiIdVector(), curl, res);
      memory.printMapOfCryptosIdAndPrice();
    }  else if (input == 'q') {
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