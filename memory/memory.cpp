// memory.cpp

#include "memory.h"
#include <iostream>

using namespace std;
size_t Memory::writeMemory(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  Memory* mem = static_cast<Memory*>(userp);
  mem->memory.insert(mem->memory.end(), (char*)contents, (char*)contents + realsize);

  return realsize;
}

string Memory::returnMemoryAsString() {
  // cout << "Rerurn memory called" << endl;
  return string(memory.begin(), memory.end());
}