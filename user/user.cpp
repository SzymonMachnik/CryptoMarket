// user.cpp

#include <iostream>
#include <fstream>

#include "user.h"

using namespace std;

User::User() {
  this->isUserLoged = false;
  this->balanceAboveWhichUserCantDepositInCents = 500000000; // 5 000 000 . 00
}

bool User::getUserLoginStatus() {
  return isUserLoged;
}

void User::loginUser() {
  string login;
  cout << "Enter login to your account: ";
  cin >> login;

  ifstream file;

  file.open(login + ".txt");

  if (file.is_open() == false) {
    cout << "Inaccurance: Account with this login doesn't exist" << endl;
    return;
  } else {
    string password;
    cout << "Enter password to your account: ";
    cin >> password;

    string line;
    getline(file, line);

    if (line == password) {
      this->isUserLoged = true;
      cout << "Loged succesful" << endl;
    } else {
      cout << "Inaccurance: Wrong password" << endl;
    }

    file.close();
  }
}

void User::registerUser() {
  string login;
  string password;
  
  bool correctLogin = false;
  do {
    cout << "Enter login to your new account: ";
    //cin.clear();
    cin.ignore(1000, '\n');
    getline(cin, login);
    if (login.find(' ') != string::npos) continue; // checking if login contains space

    // min 10 characters
    // only small letters
    correctLogin = true;
    if (login.size() < 10 || 32 < login.size()) {
      cout << "Login size must to be beteen 10 and 32 characters" << endl;
      correctLogin = false;
    } else {
      for (char c : login) {
        if (!(0 <= c - 'a' && c - 'a' <= 25)) {
          correctLogin = false;
          break;
        }
      }
    }
    
  } while (correctLogin == false);
  
  bool correctPassword = false;
  do {
    cout << "Enter password to your new account: ";
    cin.clear();
    getline(cin, password);
    if (password.find(' ') != string::npos) continue; // checking if password contains space

    correctPassword = true;
    char lowerLetterCounter = 0;
    char capitalLetterCounter = 0;
    char digitCounter = 0;
    char specialCharacterCounter = 0;

    char minLowerLetter = 4;
    char minCapitalLetter = 1;
    char minDigit = 2;
    char minSpecialCharacter = 1;

    if (password.size() < 8 || 32 < password.size()) {
      cout << "Password size must to be beteen 8 and 32 characters" << endl;
      correctPassword = false;
    } else {
      for (char c : login) {
        if (0 <= c - 'a' && c - 'a' <= 25) {
          lowerLetterCounter++;
        } else if (0 <= c - 'A' && c - 'A' <= 25) {
          capitalLetterCounter++;
        } else if (0 <= c - '0' && c - '0' <= 9) {
          digitCounter++;
        } else if (33 <= c && c <= 125) {
          specialCharacterCounter++;
        } else correctLogin = false;
      }
    }
    if (correctPassword == true) {
      if (!(lowerLetterCounter >= minLowerLetter
          && capitalLetterCounter >= minCapitalLetter
          && digitCounter >= minDigit
          && specialCharacterCounter >= minSpecialCharacter)) {

        cout << "Password has to include at least:" << endl;
        cout << minLowerLetter << " small letters" << endl;
        cout << minCapitalLetter << " capital letter" << endl;
        cout << minDigit << " digits" << endl;
        cout << minSpecialCharacter << " special character" << endl;

        correctPassword = false;
      }
    }
    
  } while (correctLogin == false);

  ofstream file;
  file.open(login + ".txt");

  file << password;

  file.close();

  this->isUserLoged = true;
  this->login = login;
  this->password = password;
}

void User::deposit(int moneyToDepositInCents) {
  
  if (balanceInCents > balanceAboveWhichUserCantDepositInCents) {
    // do nothing
  } if (balanceInCents + moneyToDepositInCents > balanceAboveWhichUserCantDepositInCents) {
    balanceInCents = balanceAboveWhichUserCantDepositInCents;
  } else {
    balanceInCents += moneyToDepositInCents;
  }
}