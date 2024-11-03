// handleUser.cpp

#include "handleUser.h"

using namespace std;


void makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                Memory &chunk, string &cryptoApiId, CURL* curl, CURLcode &result) {
  //CREATE AND REQUEST ADDRESS
  string address = "https://api.coingecko.com/api/v3/simple/price?ids=" + cryptoApiId + "&vs_currencies=usd";

  //GET DATA
  curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
  
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

void returnInaccuranceCryptoName(Crypto &crypto) {
  cout << "Inaccurance: Wrong crypto name" << endl;
  cout << "=========================================" << endl;
  cout << "List of available names of cryptos below:" << endl;
  cout << crypto.returnCryptoNamesAsString();
  cout << "=========================================" << endl;
}

string askUserForCryptoName() {
  string cryptoName = "";
  cout << "Enter the name of the crypto you wnat to know the price of.\nName: ";
  cin >> cryptoName;
  transform(cryptoName.begin(), cryptoName.end(), cryptoName.begin(), ::tolower);
  return cryptoName;
}

void askForDecisionFirstTime(string &input) {
  cout << "[1] Check price of crypto" << endl;
  cout << "[2] Quit" << endl;
  cin >> input;
}

void returnInaccurenceWrongDecision(string &input) {
  cout << "Inaccurance: Wrong input, choose one of below." << endl;
  cout << "[1] Check price of crypto" << endl;
  cout << "[2] Quit" << endl;
  // cin.clear();
  // cin.ignore(1000, '\n');
  cin >> input;
}

void askForDecision(int &decision) {
    string input;
    askForDecisionFirstTime(input); 

    // Until input isn't correct
    while (input.length() != 1 || (input != "1" && input != "2")) {
      returnInaccurenceWrongDecision(input);
    }
    
    // If input is correct convert it to an int
    decision = stoi(input);
}