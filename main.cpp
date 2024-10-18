#include <iostream>
#include <curl/curl.h>
#include "./include/json.hpp"

using namespace std;


// Funkcja callback do zapisywania odpowiedzi HTTP
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

// Funkcja do pobierania ceny BTC
std::string getBTCPrice() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        // Ustawienie URL do API CoinGecko
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=solana&vs_currencies=usd");
        //curl_easy_setopt(curl, CURLOPT_URL, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  // Wyłączenie weryfikacji SSL (dla celów testowych)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        // Ustawienie callbacka do odbierania danych
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        // Wykonanie żądania HTTP
        res = curl_easy_perform(curl);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cout << "Error: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return "";
        }
        // Sprawdzanie, czy zapytanie się powiodło
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Czyszczenie
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

int main() {
    std::string priceData = getBTCPrice();
    
    // Parsowanie JSON z odpowiedzi
    auto jsonData = nlohmann::json::parse(priceData);
    double btcPrice = jsonData["bitcoin"]["usd"];

    // Wyświetlenie ceny BTC
    std::cout << "Cena Bitcoin (BTC) w USD: " << btcPrice << std::endl;

    return 0;
}
