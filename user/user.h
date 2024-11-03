// user.h

#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


class User {
  private:
    string password;
    string login; 
    bool isUserLoged;

  public:
    User();

    bool getUserLoginStatus();
    void loginUser();
    void registerUser();
};

#endif