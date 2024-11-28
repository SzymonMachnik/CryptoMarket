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

// ImGui
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

using namespace std;

class HandleGui {
public:
  static void renderChooseRegsterOrLoginWindow(string &action);
  // static void renderRegisterWindow(char inputLogin[255], char inputPassword[255], char inputFirstName[255], char inputLastName[255],
  //                         string &inputLoginError, string &inputPasswordError, string &inputFirstNameError, string &inputLastNameError,
  //                         ImFont* errorFont,
  //                         string &action);
  // static void renderLoginWindow(char inputLogin[255], char inputPassword[255],
  //                       string &action);
  // static void renderTransactionsListSection(ImFont* transactionsListFont);

  // static void renderBuyCryptoWindow(string &buyCrypto, double &cryptoAmountToBuy, string &buyCryptoErrorMsg,
  //                           ImFont* errorFont);
  // static void renderSellCryptoWindow(string &sellCrypto, double &cryptoAmountToSell, string &sellCryptoErrorMsg,
  //                           ImFont* errorFont);
  // static void renderDepositMoneyWindow(bool &depositMoney, int &fiatToDeposit, string &depositMoneyErrorMsg,
  //                           ImFont* errorFont);
  // static void renderWalletSection(ImFont* walletFont, bool &depositMoney);

  static void renderCryptoListSection(string &buyCrypto, string &sellCrypto, Crypto &crypto);
};

#endif