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
  // string buyCryptoErrorMsg = "";
  // double cryptoAmountToBuy = 0;
// class HandleGui {
// public:
//   static void renderChooseRegsterOrLoginWindow(string &action);
//   static void renderRegisterWindow(char inputLogin[255], char inputPassword[255], char inputFirstName[255], char inputLastName[255],
//                                    string &inputLoginError, string &inputPasswordError, string &inputFirstNameError, string &inputLastNameError,
//                                    ImFont* errorFont, string &action, User &user);
//   static void renderLoginWindow(char inputLogin[255], char inputPassword[255],
//                                 string &action, User &user);

//   static void renderCryptoListSection(string &buyCrypto, string &sellCrypto, Crypto &crypto);
//   static void renderTransactionsListSection(ImFont* transactionsListFont, User &user);
//   static void renderWalletSection(ImFont* walletFont, bool &depositMoney, User &user);  

//   static void renderBuyCryptoWindow(string &buyCrypto, double &cryptoAmountToBuy, string &buyCryptoErrorMsg,
//                                     ImFont* errorFont, Crypto &crypto, User &user);
//   static void renderSellCryptoWindow(string &sellCrypto, double &cryptoAmountToSell, string &sellCryptoErrorMsg,
//                                      ImFont* errorFont, Crypto &crypto, User &user);
//   static void renderDepositMoneyWindow(bool &depositMoney, int &fiatToDeposit, string &depositMoneyErrorMsg,
//                                        ImFont* errorFont, User &user);
// };

class HandleGui {
private:
  string buyCryptoErrorMsg = "";
  double cryptoAmountToBuy = 0.;

  string sellCryptoErrorMsg = "";
  double cryptoAmountToSell = 0.;

  int fiatToDeposit = 0;
  string depositMoneyErrorMsg = "";


  char inputLogin[255] = {}; 
  char inputPassword[255] = {};
  char inputFirstName[255] = {}; 
  char inputLastName[255] = {};

  string inputLoginError;
  string inputPasswordError;
  string inputFirstNameError;
  string inputLastNameError;

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