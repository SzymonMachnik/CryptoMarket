#include <iostream>
#include <vector>
#include <string>
#include "../crypto/crypto.h"

using namespace std;



int main() {
  Crypto crypto("../crypto/listOfCryptoNames.txt");

  // cout << crypto.returnCryptoNames() << endl;

  cout << "1. EXPECTED TRUE  RETURNED: " << crypto.include("bitcoin") << endl;
  cout << "2. EXPECTED TRUE  RETURNED: " << crypto.include("avalanche") << endl;
  cout << "3. EXPECTED TRUE  RETURNED: " << crypto.include("dogecoin") << endl;
  cout << "4. EXPECTED FALSE \t RETURNED: " << crypto.include("solan") << endl;
  
  return 0;
}