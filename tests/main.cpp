#include <iostream>
#include <vector>
#include <string>
#include "../crypto/crypto.h"

using namespace std;



int main() {
  Crypto crypto("../crypto/listOfCryptoNames.txt");

  cout << crypto.returnCryptoNames() << endl;
  
  return 0;
}