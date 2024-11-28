// handlegui.cpp

#include <iostream>
#include <fstream>

#include "handlegui.h"

using namespace std;


void HandleGui::renderChooseRegsterOrLoginWindow(string &action) {
  ImGui::SetNextWindowPos(ImVec2(660, 340));
  ImGui::SetNextWindowSize(ImVec2(600, 400));
  ImGui::Begin("ChooseOptionLoginOrRegister", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.    
  
  ImGui::SetCursorPos(ImVec2(200, 100));
  if (ImGui::Button("Login", ImVec2(200, 80))) {
    action = "login";
  }
  
  ImGui::SetCursorPos(ImVec2(200, 220));
  if (ImGui::Button("Sign in", ImVec2(200, 80))) {
    action = "register";
  }

  ImGui::End();
}

void HandleGui::renderCryptoListSection(string &buyCrypto, string &sellCrypto, Crypto &crypto) {
  mutex db_mutex;

  const int containerWidth = 1280;
  const int containerHeight = 1080;
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));

  // Okno główne z możliwością przewijania
  ImGui::Begin("Crypto list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

  for (int i = 0; i < crypto.numberOfCrypto; i++)
  {
    // Wymiary rzędu kryptowaluty
    ImVec2 rowSize(containerWidth, 100);

    // Tło rzędu (opcjonalne)
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::BeginChild(std::string("CryptoRow_" + std::to_string(i)).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);

    // Wyświetlanie nazwy i ceny
    string cryptoName, cryptoPrice;
    {
      std::lock_guard<std::mutex> lock(db_mutex);
      cryptoName = crypto.getCryptoName(i + 1);
      cryptoPrice = crypto.getCryptoPrice(i + 1);
    }

    ImGui::Text(cryptoName.c_str());
    ImGui::SameLine(500); // Ustawiamy tekst w połowie szerokości
    ImGui::Text("%s $", cryptoPrice.c_str());

    ImGui::SameLine(750); 
    if (ImGui::Button("BUY", ImVec2(200, 60))) {
      buyCrypto = cryptoName;
    }
    ImGui::SameLine(970); 
    if (ImGui::Button("SELL", ImVec2(200, 60))) {
      sellCrypto = cryptoName;
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Dodanie odstępu między rzędami
    ImGui::Spacing();
  }

  ImGui::End();
}
