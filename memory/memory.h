// memory.h

#ifndef MEMORY_H
#define MEMORY_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Memory {
  private:
    vector<char> memory; 

  public:
    static size_t writeMemory(void* contents, size_t size, size_t nmemb, void* userp);

    string returnMemoryAsString();
};

#endif