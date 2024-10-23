// memory.cpp

#include <iostream>

#include "memory.h"

using namespace std;

int Memory::memorySize = 0;

size_t Memory::writeMemory(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  Memory* mem = static_cast<Memory*>(userp);
  string chunk((char*)contents, realsize);

  mem->formatStringGotFromRequest(chunk);

  mem->memory.push_back(chunk);

  memorySize++;
  
  return realsize;
}

void Memory::formatStringGotFromRequest(string &data) {
  string ans;
  int i = 0;
  //name
  for (char c : data) {
    i++;
    if (c == ':') {
      ans += c;
      break;
    } else if (c - 'a' >= 0 && c - 'a' <= 25) {
      ans += c;
    } else if (c - '0' >= 0 && c - '0' <= 9) {
      ans += c;
    } else if (c == '-') {
      ans += c;
    }
  }
  //price
  for (int j = i; j < data.size(); j++) {
    char c = data[j];
    if (c == '.') {
      ans += c;
    } else if (c - '0' >= 0 && c - '0' <= 9) {
      ans += c;
    }
  }

  data = ans;
}

string Memory::returnAllMemoryAsString() {
  // cout << "Rerurn memory called" << endl;
  string result;
  for (const auto& str : memory) {
      result += str;
  }
  return result;
}

string Memory::returnMemoryByIndex(int index) {
  if (index >= memorySize || index < 0) {
    return "";
  }
  return memory[index];
}
