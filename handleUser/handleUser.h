// handleUser.h

#ifndef HANDLEUSER_H
#define HANDLEUSER_H

#include <iostream>
#include <curl/curl.h>
#include <bits/stdc++.h> 
#include <string>
#include <bits/stdc++.h> //to transform string to lower case

#include "../memory/memory.h"
#include "../crypto/crypto.h"

using namespace std;


void makeRequestAndWriteMemory(size_t (*writeMemory)(void* contents, size_t size, size_t nmemb, void* userp), 
                                Memory &chunk, string &cryptoApiId, CURL* curl, CURLcode &result);
void returnInaccuranceCryptoName(Crypto &crypto);
string askUserForCryptoName();
void askForDecisionFirstTime(string &input);
void returnInaccurenceWrongDecision(string &input);
void askForDecision(int &decision);

#endif