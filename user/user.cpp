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

void User::saveData() {
  ofstream file;
  file.open(login + ".txt");

  file << password << '\n';
  file << balanceInCents;
  file.close(); 
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
      string line;
      getline(file, line);
      this->balanceInCents = stoi(line);
    } else {
      cout << "Inaccurance: Wrong password" << endl;
    }

    file.close();
  }

  this->login = login;
  this->password = password;
}

void User::registerUser() {
  string login;
  string password;
  
  bool correctLogin = false;
  do {
    cout << "Enter login to your new account: ";
    cin >> login;
    cin.ignore(1000, '\n');

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
    cin >> password;
    cin.ignore(1000, '\n');
    
    if (password.find(' ') != string::npos) continue; // checking if password contains space

    correctPassword = true;
    int lowerLetterCounter = 0;
    int capitalLetterCounter = 0;
    int digitCounter = 0;
    int specialCharacterCounter = 0;

    int minLowerLetter = 4;
    int minCapitalLetter = 1;
    int minDigit = 2;
    int minSpecialCharacter = 1;

    if (password.size() < 8 || 32 < password.size()) {
      cout << "Password size must to be beteen 8 and 32 characters" << endl;
      correctPassword = false;
    } else {
      for (char c : password) {
        if (0 <= c - 'a' && c - 'a' <= 25) {
          lowerLetterCounter++;
        } else if (0 <= c - 'A' && c - 'A' <= 25) {
          capitalLetterCounter++;
        } else if (0 <= c - '0' && c - '0' <= 9) {
          digitCounter++;
        } else if (33 <= c && c <= 125) {
          specialCharacterCounter++;
        } else {
          correctLogin = false;
          break;
        }
      }
    }
    if (correctPassword == true) {
      if (!(lowerLetterCounter >= minLowerLetter
          && capitalLetterCounter >= minCapitalLetter
          && digitCounter >= minDigit
          && specialCharacterCounter >= minSpecialCharacter)) {

        cout << "Password has to include at least:" << endl;
        cout << minLowerLetter << " small letters" << " Includes: " << lowerLetterCounter << endl;
        cout << minCapitalLetter << " capital letter" << " Includes: " << capitalLetterCounter << endl;
        cout << minDigit << " digits" << " Includes: " << digitCounter << endl;
        cout << minSpecialCharacter << " special character" << " Includes: " << specialCharacterCounter << endl;

        correctPassword = false;
      }
    }
    
  } while (correctPassword == false);

  this->isUserLoged = true;
  this->login = login;
  this->password = password;
  this->balanceInCents = 0;

  saveData();
}

void User::deposit(int moneyToDepositInCents) {
  if (balanceInCents > balanceAboveWhichUserCantDepositInCents) {
    // do nothing
  } else if (balanceInCents + moneyToDepositInCents > balanceAboveWhichUserCantDepositInCents) {
    balanceInCents = balanceAboveWhichUserCantDepositInCents;
    saveData();
  } else {
    balanceInCents += moneyToDepositInCents;
    saveData();
  }
}

int User::getBalanceInCents() {
  return balanceInCents;
}