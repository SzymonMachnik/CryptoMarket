// handlegui.cpp

#include <iostream>
#include <fstream>

#include "handlegui.h"

using namespace std;


string HandleGui::firstLetterToUpperCase(string text) {
  text[0] -= 32;
  return text;
}

string HandleGui::allLettersToUpperCase(string text) {
  for (size_t i = 0; i < text.size(); i++) {
    text[i] -= 32;
  }
  return text;
}

void HandleGui::renderChooseRegsterOrLoginWindow(string &action) {
  ImGui::SetNextWindowPos(ImVec2(660, 340));
  ImGui::SetNextWindowSize(ImVec2(600, 400));
  ImGui::Begin("ChooseOptionLoginOrRegister", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.    
  
  ImGui::SetCursorPos(ImVec2(200, 100));
  if (ImGui::Button("Login", ImVec2(200, 80))) {
    action = "login";
  }
  
  ImGui::SetCursorPos(ImVec2(200, 220));
  if (ImGui::Button("Register", ImVec2(200, 80))) {
    action = "register";
  }

  ImGui::End();
}

void HandleGui::renderRegisterWindow( ImFont* errorFont, string &action, User &user) {
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
  if (ImGui::Button("Register", ImVec2(200, 60))) {
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

void HandleGui::renderLoginWindow(string &action, User &user) {
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

void HandleGui::renderCryptoListSection(string &buyCrypto, string &sellCrypto, Crypto &crypto) {

  const int containerWidth = 1280;
  const int containerHeight = 1080;
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));

  ImGui::Begin("Crypto list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

  for (size_t i = 0; i < crypto.numberOfCrypto; i++)
  {
    ImVec2 rowSize(containerWidth, 100);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild(string("CryptoRow_" + to_string(i)).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);

    string cryptoName, cryptoPrice;

    cryptoName = crypto.getCryptoName(i + 1);
    cryptoPrice = crypto.getCryptoPrice(i + 1);
    
    ImGui::SetCursorPos(ImVec2(40, 27));
    ImGui::Text(firstLetterToUpperCase(cryptoName).c_str());
    ImGui::SameLine(450);
    ImGui::Text("%s $", cryptoPrice.c_str());

    // ImGui::SameLine(750); 
    ImGui::SetCursorPos(ImVec2(750, 20));
    if (ImGui::Button("BUY", ImVec2(200, 60))) {
      buyCrypto = cryptoName;
    }
    ImGui::SameLine(970); 
    // ImGui::SetCursorPos(ImVec2(970, 20));
    if (ImGui::Button("SELL", ImVec2(200, 60))) {
      sellCrypto = cryptoName;
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Spacing();
  }

  ImGui::End();
}


void HandleGui::renderTransactionsListSection(ImFont* transactionsListFont, User &user) {
  
  const int containerWidth = 640.0f;
  const int containerHeight = 591.0f;

  ImGui::SetNextWindowPos(ImVec2(1280, 489));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));

  ImGui::Begin("Transactions list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

  vector<vector<string>> transactionsList;
  transactionsList = user.returnAllTransactions();


  ImVec2 rowSize(containerWidth, 40.0f);
  ImGui::SetCursorPos(ImVec2(133, 5));
  ImGui::BeginChild("TransactionsHistory", rowSize, false, ImGuiWindowFlags_NoScrollbar);
  ImGui::Text("Transactions History");
  ImGui::EndChild();
  ImGui::Spacing();

  for (auto transaction : transactionsList)
  {
    ImVec2 rowSize(containerWidth, 110.0f);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild((string("Transaction_") + string(transaction[0])).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(transactionsListFont);

    ostringstream streamValue;
    streamValue << fixed << setprecision(2) << stoi(transaction[5]) / 100.0;

    float textLineSize = 33.0f;
    float textColumnSize = 310.0f;

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

    ImGui::Spacing();
  }

  ImGui::End();
}

void HandleGui::renderWalletSection(ImFont* walletFont, bool &depositMoney, User &user) {
  const int containerWidth = 640.0f;
  const int containerHeight = 489.0f;

  ImGui::SetNextWindowPos(ImVec2(1280, 0));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));

  ImGui::Begin("Wallet", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

  vector<vector<string>> wallet;
  wallet = user.returnAllRecordsFromWallet();

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

  ostringstream streamBalanceCents;
  streamBalanceCents << fixed << setprecision(2) << user.getBalanceInCents() / 100.;

  ImGui::Text(("Fiat balance: " + streamBalanceCents.str() + "$").c_str()); // Fiat balance
  ImGui::SetCursorPos(ImVec2(10, 6 + 33));

  int totalValueCent = 0;
  for (auto v : wallet) {
    totalValueCent += stod(v[2]);
  }
  totalValueCent += user.getBalanceInCents();

  ostringstream streamValueCents;
  streamValueCents << fixed << setprecision(2) << totalValueCent / 100.;

  ImGui::Text(("Wallet value: " + streamValueCents.str() + "$").c_str());  // Wallet value in $
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
    ImVec2 rowSize(containerWidth, 80.0f);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild((string("Transaction_") + string(record[0])).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(walletFont);

    ostringstream streamValueDolars;
    streamValueDolars << fixed << setprecision(2) << stoi(record[2]) / 100.0;

    float textLineSize = 33.0f;
    float textColumnSize = 310.0f;

    ImGui::SetCursorPos(ImVec2(10, 6));
    ImGui::Text((firstLetterToUpperCase(record[1])).c_str()); // Crypto name
    ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize));
    ImGui::Text(("Amount: " + record[3]).c_str()); // Amount

    ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6));
    ImGui::Text(("Value: " + streamValueDolars.str() + "$").c_str());  // Value in $
    ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6 + textLineSize));
    ImGui::Text(("Price: " + record[4] + "$").c_str());  // Price of crypto
    ImGui::PopFont();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Spacing();
  }

  ImGui::End();
}


void HandleGui::renderBuyCryptoWindow(string &buyCrypto, ImFont* errorFont, Crypto &crypto, User &user) {
  const int containerWidth = 600.0f;
  const int containerHeight = 300.0f;

  ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
  ImGui::SetNextWindowFocus();
  ImGui::Begin("BuyCrypto", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  
  ImGui::SetCursorPos(ImVec2(190, 10));
  ImGui::Text(("Buy " + firstLetterToUpperCase(buyCrypto)).c_str());

  ImGui::SetCursorPos(ImVec2(107, 70));
  ImGui::InputDouble("##AmountToSpend", &cryptoAmountToBuy, 0.1, 1.0, "%.4f");
  cryptoAmountToBuy = max(0.0, cryptoAmountToBuy);
  if (cryptoAmountToBuy < 0.0001) cryptoAmountToBuy = 0.0;
  int valueCent = 0;

  if (crypto.getCryptoPrice(buyCrypto).empty() == false && cryptoAmountToBuy != 0.0) {
    valueCent = cryptoAmountToBuy * 100 * stod(crypto.getCryptoPrice(buyCrypto)) + 1;
  }

  ostringstream ss_valueDolars;
  if (cryptoAmountToBuy != 0.0) {
    ss_valueDolars << fixed << setprecision(2) << valueCent / 100.;
  } else {
    ss_valueDolars << 0;
  }

  ImGui::SetCursorPos(ImVec2(107, 125));
  ImGui::PushFont(errorFont);
  ImGui::Text((buyCryptoErrorMsg).c_str());
  ImGui::PopFont();
  
  ImGui::SetCursorPos(ImVec2(150, 150));
  ImGui::Text(("Cost " + (ss_valueDolars).str() + "$").c_str());

  ImGui::SetCursorPos(ImVec2(200, 220));
  if (ImGui::Button("BUY", ImVec2(200, 60))) {
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

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    buyCrypto = "";
    cryptoAmountToBuy = 0;
    buyCryptoErrorMsg = "";
  }

  ImGui::PopStyleColor();
  ImGui::End();
}

void HandleGui::renderSellCryptoWindow(string &sellCrypto, ImFont* errorFont, Crypto &crypto, User &user) {
  const int containerWidth = 600.0f;
  const int containerHeight = 300.0f;

  ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
  ImGui::SetNextWindowFocus();
  ImGui::Begin("SellCrypto", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  
  ImGui::SetCursorPos(ImVec2(190, 10));
  ImGui::Text(("Sell " + firstLetterToUpperCase(sellCrypto)).c_str());

  ImGui::SetCursorPos(ImVec2(107, 70));
  ImGui::InputDouble("##AmountToSpend", &cryptoAmountToSell, 0.1, 1.0, "%.4f");
  cryptoAmountToSell = max(0.0, cryptoAmountToSell);
  if (cryptoAmountToSell < 0.0001) cryptoAmountToSell = 0.0;
  int valueCent = 0;

  if (crypto.getCryptoPrice(sellCrypto).empty() == false) {
    valueCent = cryptoAmountToSell * 100 * stod(crypto.getCryptoPrice(sellCrypto)) - 1;
  }

  ostringstream ss_valueDolars;
  if (cryptoAmountToSell != 0) {
    ss_valueDolars << fixed << setprecision(2) << valueCent / 100.;
  } else {
    ss_valueDolars << 0;
  }
  
  ImGui::SetCursorPos(ImVec2(107, 125));
  ImGui::PushFont(errorFont);
  ImGui::Text((sellCryptoErrorMsg).c_str());
  ImGui::PopFont();

  ImGui::SetCursorPos(ImVec2(150, 150));
  ImGui::Text(("Cost " + (ss_valueDolars).str() + "$").c_str());

  ImGui::SetCursorPos(ImVec2(200, 220));
  if (ImGui::Button("SELL", ImVec2(200, 60))) {
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

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    sellCrypto = "";
    cryptoAmountToSell = 0;
    sellCryptoErrorMsg = "";
  }

  ImGui::PopStyleColor();
  ImGui::End();
}

void HandleGui::renderDepositMoneyWindow(bool &depositMoney,  ImFont* errorFont, User &user) {
  const int containerWidth = 600.0f;
  const int containerHeight = 300.0f;

  ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
  ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
  ImGui::SetNextWindowFocus();
  ImGui::Begin("DepositMoney", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  
  ImGui::SetCursorPos(ImVec2(190, 10));
  ImGui::Text("Deposit money");

  ImGui::SetCursorPos(ImVec2(107, 80));
  ImGui::InputInt("##InputMoneyToDeposit", &fiatToDeposit, 100, 1000);

  ImGui::SetCursorPos(ImVec2(107, 135));
  ImGui::PushFont(errorFont);
  ImGui::Text((depositMoneyErrorMsg).c_str());
  ImGui::PopFont();

  ImGui::SetCursorPos(ImVec2(200, 190));
  if (ImGui::Button("DEPOSIT", ImVec2(200, 80))) {
    fiatToDeposit = max(0, fiatToDeposit);
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

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    depositMoney = false;
    fiatToDeposit = 0;
    depositMoneyErrorMsg = "";
  }

  ImGui::PopStyleColor();
  ImGui::End();
}