// memory.h

#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Memory {
  private:
    vector<string> memory; 

  public:
    static int memorySize;
    static size_t writeMemory(void* contents, size_t size, size_t nmemb, void* userp);

    string returnAllMemoryAsString();
    string returnMemoryByIndex(int index);

    void formatStringGotFromRequest(string &data);
};

#endif