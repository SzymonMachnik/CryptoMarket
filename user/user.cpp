// user.cpp

#include <iostream>
#include <fstream>

#include "user.h"

using namespace std;


bool User::isUserLoged = false;

void User::loginUser {
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
      isUserLoged = true;
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

  do {
    cout << "Enter login to your new account: ";
    cin >> login;
    bool correctLogin = false;
    //1 znak specjalny
    //2 cyfry
    //1 duża litera
    //6 małych znaków
    //0 brak spacji
  } while (correctLogin == false);
  

  cout << "Enter password to your new account: ";
  cin >> password;

  ofstream file;
  file.open(login + ".txt");

  file << password;

  file.close();

  isUserLoged = true;
  this->login = login;
  this->password = password;
}