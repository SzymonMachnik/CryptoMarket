#include <iostream>
#include <curl/curl.h>
#include <vector>

using namespace std;

struct MemoryStruct {
  vector<char> memory; 
};

size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  size_t realsize = size * nmemb;
  MemoryStruct* mem = (MemoryStruct*)userp;

  mem->memory.insert(mem->memory.end(), (char*)contents, (char*)contents + realsize);

  return realsize;
}

int main() {
  CURL* curl;
  CURLcode result;

  MemoryStruct chunk;

  // Inicjalizacja cURL
  curl = curl_easy_init();
  if (curl == nullptr) {
      cout << "HTTP request initialization failed" << endl;
      return -1;
  }

  // Ustawienie opcji dla żądania cURL
  curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); 
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk); 

  // Wykonanie żądania HTTP
  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    cout << "Error: " << curl_easy_strerror(result) << endl;
    curl_easy_cleanup(curl);
    return -1;
  }

  // Dodanie null-terminatora do wektora, aby stworzyć poprawny ciąg znaków
  chunk.memory.push_back('\0');

  // Wyświetlenie odpowiedzi
  cout << chunk.memory.data() << " <-- response string" << endl;

  // Zwolnienie zasobów cURL
  curl_easy_cleanup(curl);

  return 0;
}