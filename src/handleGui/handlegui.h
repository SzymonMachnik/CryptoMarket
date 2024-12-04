#ifndef HANDLEGUI_H
#define HANDLEGUI_H

#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <sstream>
#include <mutex>
#include <iomanip>
#include "../crypto/crypto.h"
#include "../user/user.h"
// ImGui
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

using namespace std;


class HandleGui {
private:
  char inputLogin[255] = {}; 
  char inputPassword[255] = {};
  char inputFirstName[255] = {}; 
  char inputLastName[255] = {};
  string inputLoginError;
  string inputPasswordError;
  string inputFirstNameError;
  string inputLastNameError;

  string buyCryptoErrorMsg = "";
  double cryptoAmountToBuy = 0.;

  string sellCryptoErrorMsg = "";
  double cryptoAmountToSell = 0.;

  int fiatToDeposit = 0;
  string depositMoneyErrorMsg = "";

  string firstLetterToUpperCase(string text);
  string allLettersToUpperCase(string text);

public:
  void renderChooseRegsterOrLoginWindow(string &action);
  void renderRegisterWindow(ImFont* errorFont, string &action, User &user);
  void renderLoginWindow(string &action, User &user);

  void renderCryptoListSection(string &buyCrypto, string &sellCrypto, Crypto &crypto);
  void renderTransactionsListSection(ImFont* transactionsListFont, User &user);
  void renderWalletSection(ImFont* walletFont, bool &depositMoney, User &user);  

  void renderBuyCryptoWindow(string &buyCrypto, ImFont* errorFont, Crypto &crypto, User &user);
  void renderSellCryptoWindow(string &sellCrypto, ImFont* errorFont, Crypto &crypto, User &user);
  void renderDepositMoneyWindow(bool &depositMoney, ImFont* errorFont, User &user);
};

#endif