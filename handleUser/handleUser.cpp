// handleUser.cpp

#include "handleUser.h"

using namespace std;


int formatStringToMoneyInCentsToDeposit(string moneyToFormat) {
  int moneyInCents = 0;
  for (char c : moneyToFormat) {
    if (0 <= c - '0' && c - '0' <= 9) {
      moneyInCents = moneyInCents * 10 + (c - '0');
    } else return 0;
  }
  return moneyInCents * 100;
}