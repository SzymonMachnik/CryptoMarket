#include <iostream>
#include <curl/curl.h>
#include <string>
#include <map>
#include <cassert>
#include <sqlite3.h>
#include "../memory/memory.h"
#include "../crypto/crypto.h"
#include "../user/user.h"
#include "../handleUser/handleUser.h"

using namespace std;


int main() {
  Crypto crypto("../crypto/listOfCryptoNames.txt", "../crypto/listOfCryptoApiIds.txt");


  //TEST1 GETCRYPTONAMEANDIDMAP////////////////////////////////////////////////////////////////////////////////////
  // map<string, string> map;
  // map = crypto.getCryptoApiIdAndNameMap();
  // for (auto it : map) {
  //   cout << it.first << " : " << it.second << endl;
  // }

  //TEST2 GETCRYPTONAMEVECTOR & GETCRYPTOAPIIDVECTOR//////////////////////////////////////////////////////////////
  // vector<string> cryptoNameVector;
  // vector<string> cryptoApiIdVector;
  // cryptoNameVector = crypto.getCryptoNameVector();
  // cryptoApiIdVector = crypto.getCryptoApiIdVector();

  // for (auto n : cryptoNameVector) {
  //   cout << n << endl;
  // }
  // cout << endl;
  // for (auto a : cryptoApiIdVector) {
  //   cout << a << endl;
  // }
  //

  //TEST3 FORMATSTRINGRECEIVEDFROMREQUEST() /////////////////////////////////////////////////////////////////////////
  // Memory memory;
  // string data = "{\"avalanche-2\":{\"usd\":24.09},\"binancecoin\":{\"usd\":568.55},\"bitcoin\":{\"usd\":69412},\"cardano\":{\"usd\":0.352065},\"dogecoin\":{\"usd\":0.158325},\"ethereum\":{\"usd\":2487.54},\"ripple\":{\"usd\":0.509382},\"solana\":{\"usd\":163.76},\"the-open-network\":{\"usd\":4.86},\"tron\":{\"usd\":0.166085}}";
  // memory.formatStringReceivedFromRequestToMap(data);
  // cout << data << endl;

  // TEST4 USER REGISTER() ////////////////////////////////////////////////////////////////////////////////////////////
  // User user;
  // user.registerUser();
  
  // TEST5 USER formatStringToMoneyInCentsToDeposit() //////////////////////////////////////////////////////////////////
  // Test 1: Standard case
  // assert(formatStringToMoneyInCentsToDeposit("123") == 12300); // 123 zł -> 12300 cents

  // // Test 2: Amount with zero
  // assert(formatStringToMoneyInCentsToDeposit("0") == 0); // 0 zł -> 0 cents

  // // Test 3: Amount with multiple digits
  // assert(formatStringToMoneyInCentsToDeposit("4567") == 456700); // 4567 zł -> 456700 cents

  // // Test 4: Invalid format (contains non-digit characters)
  // assert(formatStringToMoneyInCentsToDeposit("12a34") == 0); // Invalid -> 0 cents

  // // Test 5: Empty string
  // assert(formatStringToMoneyInCentsToDeposit("") == 0); // Empty -> 0 cents

  // // Test 6: Amount with leading zeros
  // assert(formatStringToMoneyInCentsToDeposit("000123") == 12300); // 000123 zł -> 12300 cents

  // // Test 7: Large amount
  // assert(formatStringToMoneyInCentsToDeposit("100000") == 10000000); // 100000 zł -> 10000000 cents

  // // Test 8: Contains spaces (should be invalid)
  // assert(formatStringToMoneyInCentsToDeposit("12 34") == 0); // Invalid -> 0 cents

  // // Test 9: Contains special characters
  // assert(formatStringToMoneyInCentsToDeposit("123$") == 0); // Invalid -> 0 cents

  // // Test 10: Negative numbers (if such cases should be considered invalid)
  // assert(formatStringToMoneyInCentsToDeposit("-123") == 0); // Invalid -> 0 cents

  // cout << "All tests completed successfully!" << endl;

  // TEST6 USER doesLoginExistInDb() ////////////////////////////////////////////////////////////////////////////////////
  
  User user;

  cout << "Test1 \t Expected: True \t Returned: " << user.doesLoginExistInDb("szymonmachnik") << endl;
  cout << "Test2 \t Expected: True \t Returned: " << user.doesLoginExistInDb("szymonmach") << endl;
  cout << "Test3 \t Expected: False \t Returned: " << user.doesLoginExistInDb("szymonmachh") << endl;
  cout << "Test4 \t Expected: False \t Returned: " << user.doesLoginExistInDb("") << endl;


  return 0;
}