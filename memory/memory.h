// memory.h

#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <map>

using namespace std;


class Memory {
  private:
    //map<string, float> mapOfCryptosIdAndPrice

  public:
    string chunk;
    void makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                   vector<string> apiId, CURL* curl, CURLcode &result);
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void formatStringReceivedFromRequestToMap(string data);
};

#endif