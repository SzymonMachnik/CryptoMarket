// handlegui.cpp

#include <iostream>
#include <fstream>

#include "handlegui.h"

using namespace std;


void HandleGui::renderChooseRegsterOrLoginWindow(string &action) {
  ImGui::SetNextWindowPos(ImVec2(660, 340));
  ImGui::SetNextWindowSize(ImVec2(600, 400));
  ImGui::Begin("ChooseOptionLoginOrRegister", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.    
  
  ImGui::SetCursorPos(ImVec2(200, 100));
  if (ImGui::Button("Login", ImVec2(200, 80))) {
    action = "login";
  }
  
  ImGui::SetCursorPos(ImVec2(200, 220));
  if (ImGui::Button("Sign in", ImVec2(200, 80))) {
    action = "register";
  }

  ImGui::End();
}

void HandleGui::renderCryptoListSection(string &buyCrypto, string &sellCrypto, Crypto &crypto) {
  mutex db_mutex;

  const int containerWidth = 1280;
  const int containerHeight = 1080;
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));

  // Okno główne z możliwością przewijania
  ImGui::Begin("Crypto list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

  for (int i = 0; i < crypto.numberOfCrypto; i++)
  {
    // Wymiary rzędu kryptowaluty
    ImVec2 rowSize(containerWidth, 100);

    // Tło rzędu (opcjonalne)
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild(std::string("CryptoRow_" + std::to_string(i)).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);

    // Wyświetlanie nazwy i ceny
    string cryptoName, cryptoPrice;
    {
      std::lock_guard<std::mutex> lock(db_mutex);
      cryptoName = crypto.getCryptoName(i + 1);
      cryptoPrice = crypto.getCryptoPrice(i + 1);
    }

    ImGui::Text(cryptoName.c_str());
    ImGui::SameLine(500); // Ustawiamy tekst w połowie szerokości
    ImGui::Text("%s $", cryptoPrice.c_str());

    ImGui::SameLine(750); 
    if (ImGui::Button("BUY", ImVec2(200, 60))) {
      buyCrypto = cryptoName;
    }
    ImGui::SameLine(970); 
    if (ImGui::Button("SELL", ImVec2(200, 60))) {
      sellCrypto = cryptoName;
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Dodanie odstępu między rzędami
    ImGui::Spacing();
  }

  ImGui::End();
}

void HandleGui::renderRegisterWindow(char inputLogin[255], char inputPassword[255], char inputFirstName[255], char inputLastName[255],
                          string &inputLoginError, string &inputPasswordError, string &inputFirstNameError, string &inputLastNameError,
                          ImFont* errorFont, string &action, User &user) {
  ImGui::SetNextWindowPos(ImVec2(660, 190));
  ImGui::SetNextWindowSize(ImVec2(600, 650));

  ImGui::Begin("Register", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.

  ImGui::SetCursorPos(ImVec2(20, 20));
  ImGui::Text("Login:");

  ImGui::SetCursorPos(ImVec2(20, 70));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##logininput", inputLogin, CHAR_MAX);

  if (inputLoginError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 125));
    ImGui::Text(inputLoginError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(20, 150));
  ImGui::Text("Password:");

  ImGui::SetCursorPos(ImVec2(20, 200));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##passwordinput", inputPassword, CHAR_MAX, ImGuiInputTextFlags_Password);

  if (inputPasswordError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 255));
    ImGui::Text(inputPasswordError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(20, 300));
  ImGui::Text("First name:");

  ImGui::SetCursorPos(ImVec2(20, 350));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##firstnameinput", inputFirstName, CHAR_MAX);

  if (inputFirstNameError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 405));
    ImGui::Text(inputFirstNameError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(20, 430));
  ImGui::Text("Last name:");

  ImGui::SetCursorPos(ImVec2(20, 480));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##lastnameinput", inputLastName, CHAR_MAX);

  if (inputLastNameError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 535));
    ImGui::Text(inputLastNameError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(200, 570));
  if (ImGui::Button("Sign in", ImVec2(200, 60))) {
    int errorCode = user.registerUser(inputLogin, inputPassword, inputFirstName, inputLastName);
    inputLoginError = "";
    inputPasswordError = "";
    inputFirstNameError = "";
    inputLastNameError = "";

    if (errorCode != 0) {
      if (errorCode < 10) {
        if (errorCode == 1) inputLoginError = "Login must be between 8 and 32 characters.";
        else if (errorCode == 2) inputLoginError = "Login must contain only letters.";
        else if (errorCode == 3) inputLoginError = "Account with this login already exists.";
        else inputLoginError = "Wrong login.";
        inputLogin[0] = '\0'; 

      } else if (errorCode < 100) {
        errorCode %= 10;
        if (errorCode == 1) inputPasswordError = "Password must be between 8 and 32 characters.";
        else if (errorCode == 2) inputPasswordError = "Password must contain only lower, capital letters,\ndigits and special characters.";
        else if (errorCode == 3) inputPasswordError = "Password must contain at least 4 lower letters, 1 capital letter,\n2 digits and 1 special character.";
        else inputPasswordError = "Wrong password.";
        inputPassword[0] = '\0';
      } else if (errorCode < 1000) {
        inputFirstNameError = "Enter first name.";
        inputFirstName[0] = '\0'; 

      } else {
        inputLastNameError = "Enter last name.";
        inputLastName[0] = '\0';
      }
    }
  }

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    action = "chooseLoginOrRegister";
  }

  ImGui::End();
}

void HandleGui::renderLoginWindow(char inputLogin[255], char inputPassword[255],
                       string &action, User &user) {
  ImGui::SetNextWindowPos(ImVec2(660, 290));
  ImGui::SetNextWindowSize(ImVec2(600, 400));

  ImGui::Begin("Login", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.

  ImGui::SetCursorPos(ImVec2(20, 20));
  ImGui::Text("Login:");
  
  ImGui::SetCursorPos(ImVec2(20, 80));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##logininput", inputLogin, CHAR_MAX);
  
  ImGui::SetCursorPos(ImVec2(20, 150));
  ImGui::Text("Password:");

  ImGui::SetCursorPos(ImVec2(20, 210));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##passwordinput", inputPassword, CHAR_MAX, ImGuiInputTextFlags_Password);

  ImGui::SetCursorPos(ImVec2(200, 300));
  // ImGui::SetNextItemWidth(200);
  if (ImGui::Button("Login", ImVec2(200, 60))) {
    user.loginUser(inputLogin, inputPassword);
    if (user.getUserLoginStatus() == false) {
      inputLogin[0] = '\0'; 
      inputPassword[0] = '\0';
    }
  }

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    action = "chooseLoginOrRegister";
  }


  ImGui::End();
  
}


void HandleGui::renderTransactionsListSection(ImFont* transactionsListFont, User &user) {
  mutex db_mutex;

  const int containerWidth = 640.0f;
  const int containerHeight = 591.0f;

  ImGui::SetNextWindowPos(ImVec2(1280, 489));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));



  ImGui::Begin("Transactions list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight));

  vector<vector<string>> transactionsList;
  {
    std::lock_guard<std::mutex> lock(db_mutex);
    transactionsList = user.returnAllTransactions();
  }

  ImVec2 rowSize(containerWidth, 40.0f);
  ImGui::SetCursorPos(ImVec2(133, 5));
  ImGui::BeginChild("TransactionsHistory", rowSize, false, ImGuiWindowFlags_NoScrollbar);
  ImGui::Text("Transactions History");
  ImGui::EndChild();
  ImGui::Spacing();

  for (auto transaction : transactionsList)
  {
    // Wymiary rzędu kryptowaluty
    ImVec2 rowSize(containerWidth, 110.0f);

    // Tło rzędu (opcjonalne)
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild((std::string("Transaction_") + std::string(transaction[0])).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(transactionsListFont);

    std::ostringstream streamValue;
    streamValue << std::fixed << std::setprecision(2) << stoi(transaction[5]) / 100.0;

    float textLineSize = 33.0f;
    float textColumnSize = 310.0f;

    auto firstLetterToUpperCase = [](string text) -> string {
      text[0] -= 32;
      return text;
    };
    auto allLettersToUpperCase = [](string text) -> string {
      for (int i = 0; i < text.size(); i++) {
        text[i] -= 32;
      }
      return text;
    };

    ImGui::SetCursorPos(ImVec2(10, 6));
    ImGui::Text((allLettersToUpperCase(transaction[6]) + " " + firstLetterToUpperCase(transaction[2])).c_str()); // Type + crypto name
    ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize));
    ImGui::Text(("Amount: " + transaction[3]).c_str()); // Amount
    ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize * 2));
    ImGui::Text(("Value: " + streamValue.str() + "$").c_str()); // Value in $

    ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6));
    ImGui::Text((transaction[1]).c_str());  // Date of transaction
    ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6 + textLineSize));
    ImGui::Text(("Price: " + transaction[4] + "$").c_str());  // Price of crypto
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Dodanie odstępu między rzędami
    ImGui::Spacing();
  }

  ImGui::End();
}

void HandleGui::renderWalletSection(ImFont* walletFont, bool &depositMoney, User &user) {
  mutex db_mutex;
  
  const int containerWidth = 640.0f;
  const int containerHeight = 489.0f;

  ImGui::SetNextWindowPos(ImVec2(1280, 0));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));



  ImGui::Begin("Wallet", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight));

  vector<vector<string>> wallet;
  {
    std::lock_guard<std::mutex> lock(db_mutex);
    wallet = user.returnAllRecordsFromWallet();
  }

  ImVec2 rowSize(containerWidth, 40.0f);
  ImGui::BeginChild("WalletHeader", rowSize, false, ImGuiWindowFlags_NoScrollbar);
  ImGui::SetCursorPos(ImVec2(264, 0));
  ImGui::Text("Wallet");
  ImGui::EndChild();
  ImGui::Spacing();

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
  ImGui::PushFont(walletFont);
  ImGui::BeginChild("WalletMainInfo", ImVec2(containerWidth, 80.0f), false, ImGuiWindowFlags_NoScrollbar);
  ImGui::SetCursorPos(ImVec2(10, 6));
  ImGui::SetCursorPos(ImVec2(10, 6));
  std::ostringstream streamValue1;
  streamValue1 << std::fixed << std::setprecision(2) << user.getBalanceInCents() / 100.;
  ImGui::Text(("Fiat balance: " + streamValue1.str() + "$").c_str()); // Fiat balance
  ImGui::SetCursorPos(ImVec2(10, 6 + 33));

  int totalValueCent = 0;
  for (auto v : wallet) {
    totalValueCent += stod(v[2]);
  }
  totalValueCent += user.getBalanceInCents();
  std::ostringstream streamValue2;
  streamValue2 << std::fixed << std::setprecision(2) << totalValueCent / 100.;
  ImGui::Text(("Wallet value: " + streamValue2.str() + "$").c_str());  // Wallet value in $
  ImGui::PopFont();
  ImGui::PopStyleColor();

  ImGui::PushFont(walletFont);
  ImGui::SetCursorPos(ImVec2(410, 10));
  if (ImGui::Button("Deposit money", ImVec2(200, 60))) {
    depositMoney = true;
  }
  ImGui::PopFont();
  

  ImGui::EndChild();
  ImGui::Spacing();

  for (auto record : wallet)
  {
    // Wymiary rzędu kryptowaluty
    ImVec2 rowSize(containerWidth, 80.0f);

    // Tło rzędu (opcjonalne)
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild((std::string("Transaction_") + std::string(record[0])).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(walletFont);

    std::ostringstream streamValue;
    streamValue << std::fixed << std::setprecision(2) << stoi(record[2]) / 100.0;

    float textLineSize = 33.0f;
    float textColumnSize = 310.0f;

    auto firstLetterToUpperCase = [](string text) -> string {
      text[0] -= 32;
      return text;
    };

    ImGui::SetCursorPos(ImVec2(10, 6));
    ImGui::Text((firstLetterToUpperCase(record[1])).c_str()); // Crypto name
    ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize));
    ImGui::Text(("Amount: " + record[3]).c_str()); // Amount

    ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6));
    ImGui::Text(("Value: " + streamValue.str() + "$").c_str());  // Value in $
    ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6 + textLineSize));
    ImGui::Text(("Price: " + record[4] + "$").c_str());  // Price of crypto
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Dodanie odstępu między rzędami
    ImGui::Spacing();
  }

  ImGui::End();
}


void HandleGui::renderBuyCryptoWindow(string &buyCrypto, double &cryptoAmountToBuy, string &buyCryptoErrorMsg,
                           ImFont* errorFont, Crypto &crypto, User &user) {
  const int containerWidth = 600.0f;
  const int containerHeight = 300.0f;

  ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
  ImGui::SetNextWindowFocus();
  ImGui::Begin("BuyCrypto", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text(("Buy " + buyCrypto).c_str());
  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    buyCrypto = "";
    cryptoAmountToBuy = 0;
    buyCryptoErrorMsg = "";
  }
  ImGui::Spacing();
  ImGui::InputDouble("##AmountToSpend", &cryptoAmountToBuy, 0.1, 1.0, "%.4f");
  cryptoAmountToBuy = std::max(0.0, cryptoAmountToBuy);
  if (cryptoAmountToBuy < 0.0001) cryptoAmountToBuy = 0.0;
  int valueCent = 0;
  if (crypto.getCryptoPrice(buyCrypto).empty() == false && cryptoAmountToBuy != 0.0) {
    valueCent = cryptoAmountToBuy * 100 * stod(crypto.getCryptoPrice(buyCrypto)) + 1;
  }
  std::ostringstream ss_valueDolars;
  if (cryptoAmountToBuy != 0.0) {
    ss_valueDolars << std::fixed << std::setprecision(2) << valueCent / 100.;
  } else {
    ss_valueDolars << 0;
  }
  
  ImGui::Text(("Value " + (ss_valueDolars).str() + "$").c_str());
  ImGui::PushFont(errorFont);
  ImGui::Text((buyCryptoErrorMsg).c_str());
  ImGui::PopFont();
  if (ImGui::Button("BUY", ImVec2(200, 80))) {
    int buyCryptoError = user.buyCrypto(crypto.getCryptoId(buyCrypto), cryptoAmountToBuy, valueCent);
    if (buyCryptoError == 0) {
      buyCrypto = "";
      cryptoAmountToBuy = 0;
      buyCryptoErrorMsg = "";
    } else if (buyCryptoError == 1) {
      buyCryptoErrorMsg = "You can't afford it.";
    } else if (buyCryptoError == 2) {
      buyCryptoErrorMsg = "Enter amount of crypto.";
    } else {
      buyCryptoErrorMsg = "Undefined error.";
    }

  }
  ImGui::PopStyleColor();
  ImGui::End();
}

void HandleGui::renderSellCryptoWindow(string &sellCrypto, double &cryptoAmountToSell, string &sellCryptoErrorMsg,
                           ImFont* errorFont, Crypto &crypto, User &user) {
  const int containerWidth = 600.0f;
  const int containerHeight = 300.0f;

  ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
  ImGui::SetNextWindowFocus();
  ImGui::Begin("SellCrypto", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text(("Sell " + sellCrypto).c_str());
  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    sellCrypto = "";
    cryptoAmountToSell = 0;
    sellCryptoErrorMsg = "";
  }
  ImGui::Spacing();
  ImGui::InputDouble("##AmountToSpend", &cryptoAmountToSell, 0.1, 1.0, "%.4f");
  cryptoAmountToSell = std::max(0.0, cryptoAmountToSell);
  if (cryptoAmountToSell < 0.0001) cryptoAmountToSell = 0.0;
  int valueCent = 0;
  if (crypto.getCryptoPrice(sellCrypto).empty() == false) {
    valueCent = cryptoAmountToSell * 100 * stod(crypto.getCryptoPrice(sellCrypto)) - 1;
  }
  std::ostringstream ss_valueDolars;
  if (cryptoAmountToSell != 0) {
    ss_valueDolars << std::fixed << std::setprecision(2) << valueCent / 100.;
  } else {
    ss_valueDolars << 0;
  }
  
  ImGui::Text(("Value " + (ss_valueDolars).str() + "$").c_str());
  ImGui::PushFont(errorFont);
  ImGui::Text((sellCryptoErrorMsg).c_str());
  ImGui::PopFont();
  if (ImGui::Button("SELL", ImVec2(200, 80))) {
    int sellCryptoError = user.sellCrypto(crypto.getCryptoId(sellCrypto), cryptoAmountToSell, valueCent);
    if (sellCryptoError == 0) {
      sellCrypto = "";
      cryptoAmountToSell = 0;
      sellCryptoErrorMsg = "";
    } else if (sellCryptoError == 1) {
      sellCryptoErrorMsg = "Enter amount of crypto.";
    } else if (sellCryptoError == 2) {
      sellCryptoErrorMsg = "You don't have enough crypto in wallet.";
    } else {
      sellCryptoErrorMsg = "Undefined error.";
    }
  }
  ImGui::PopStyleColor();
  ImGui::End();
}

void HandleGui::renderDepositMoneyWindow(bool &depositMoney, int &fiatToDeposit, string &depositMoneyErrorMsg,
                           ImFont* errorFont, User &user) {
  const int containerWidth = 600.0f;
  const int containerHeight = 300.0f;

  ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
  ImGui::SetNextWindowFocus();
  ImGui::Begin("DepositMoney", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::Text("Deposit money");
  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    depositMoney = false;
    fiatToDeposit = 0;
    depositMoneyErrorMsg = "";
  }
  ImGui::Spacing();
  ImGui::InputInt("##InputMoneyToDeposit", &fiatToDeposit, 100, 1000);

  ImGui::PushFont(errorFont);
  ImGui::Text((depositMoneyErrorMsg).c_str());
  ImGui::PopFont();

  if (ImGui::Button("DEPOSIT", ImVec2(200, 80))) {
    fiatToDeposit = std::max(0, fiatToDeposit);
    int fiatToDepositCent = fiatToDeposit * 100;
    
    int depositMoneyError = user.deposit(fiatToDepositCent);
    
    if (depositMoneyError == 0) {
      depositMoneyErrorMsg = "";
      depositMoney = false;
    } else if (depositMoneyError == 1 || depositMoneyError == 3) {
      depositMoneyErrorMsg = "Max fiat balance is 1 000 000$.";
    } else if (depositMoneyError == 2) {
      depositMoneyErrorMsg = "Enter value from 1 - 1 000 000$.";
    }
    fiatToDeposit = 0;

  }
  ImGui::PopStyleColor();
  ImGui::End();
}

