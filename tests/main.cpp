#include <iostream>
#include <curl/curl.h>
#include <string>
#include <map>
#include <cassert>
#include <sqlite3.h>
#include "../memory/memory.h"
#include "../crypto/crypto.h"
#include "../user/user.h"

using namespace std;


int main() {
  Crypto crypto("../crypto/listOfCryptoNames.txt", "../crypto/listOfCryptoApiIds.txt");

  return 0;
}