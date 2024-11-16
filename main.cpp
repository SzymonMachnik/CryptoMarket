#include <iostream>
#include <curl/curl.h>
#include <sqlite3.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#include "crypto/crypto.h"
#include "memory/memory.h"
#include "user/user.h"
#include "handleUser/handleUser.h"
using namespace std;


// Init
CURL* curl;
CURLcode res;
Memory memory;
Crypto crypto;  
User user;


atomic<bool> keep_running(true); // Flag to stop thread

void refreshAndPrintPriceEvery60s() {
  while (keep_running) {
    memory.makeRequestAndWriteMemory(crypto.getCryptoApiIdVector(), curl, res);
    //cout << "Crypto price updated" << endl;
    this_thread::sleep_for(chrono::seconds(61)); // Wait 60s for price refresh
  } 
}


int main() {
  // Login user
  while (user.getUserLoginStatus() == false) {
    string input;
    cout << "[1] Login to your account" << endl;
    cout << "[2] Create account" << endl;
    cout << "[3] Quit" << endl;
    cout << "Input: ";
    cin >> input;
    cin.ignore(1000, '\n');

    if (input == "1") {
      user.loginUser();
    } else if (input == "2") {
      user.registerUser();
    } else if (input == "3") {
      return 0;
    } else {
      cout << "Inaccurance: Wrong input" << endl;
    }
  }


  // Curl init
  curl = curl_easy_init();
  if (!curl) {
    cout << "ERROR: Failed to initialize CURL." << endl;
    return 1;
  }
  thread messageThread(refreshAndPrintPriceEvery60s);
  

  // Main loop
  while (true) {
    string input;
    cout << "[1] Buy a crypto" << endl;
    cout << "[2] Check price of cryptos" << endl;
    cout << "[3] Deposit money to your account" << endl;
    cout << "[4] Check current balance" << endl;
    cout << "[9] Quit" << endl;
    cout << "Input: ";
    cin >> input;
    cin.ignore(1000, '\n');

    if (input == "1") {
      user.buyCrypto();
    } else if (input == "2") {
      memory.printCryptoNameAndPriceDb();
    } else if (input == "3") {
      string money;
      cout << "Amount to money to deposit: ";
      cin >> money;
      cin.ignore(1000, '\n');

      int formattedMoney; 
      formattedMoney = formatStringToMoneyInCentsToDeposit(money);
      if (formattedMoney > 0) user.deposit(formattedMoney);
    } else if (input == "4") {
      cout << "Your current balance: " << user.getBalanceInCents() / 100 << " $" << endl;
    } else if (input == "9") {
      cout << "Wait for close the application. This can take max 60 seconds." << endl;
      keep_running = false;
      break;
    } else {
      cout << "Inaccurance: Wrong input" << endl;
    }
  }


  // if (messageThread.joinable()) {
  //   messageThread.join();
  // }

  // Czyszczenie zasobów CURL
  curl_easy_cleanup(curl);

  cout << "Finished succesful." << endl;

  return 0;
}