#include <iostream>
#include <curl/curl.h>
#include <sqlite3.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <iomanip>

#include "crypto/crypto.h"
#include "memory/memory.h"
#include "user/user.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

using namespace std;


// Init
CURL* curl;
CURLcode res;
Memory memory;
Crypto crypto;  
User user;

mutex db_mutex;


atomic<bool> keep_running(true); // Flag to stop thread
void refreshAndPrintPriceEvery60s() {
  while (keep_running) {
    {
      std::lock_guard<std::mutex> lock(db_mutex);
      memory.makeRequestAndWriteMemory(crypto.getCryptoApiIdVector(), curl, res);
    }
    this_thread::sleep_for(chrono::seconds(61)); // Wait 60s for price refresh
  } 
}
void renderChooseRegsterOrLoginWindow(string &action);
void renderRegisterWindow(char inputLogin[255], char inputPassword[255], char inputFirstName[255], char inputLastName[255],
                          string &inputLoginError, string &inputPasswordError, string &inputFirstNameError, string &inputLastNameError,
                          ImFont* errorFont,
                          string &action);
void renderLoginWindow(char inputLogin[255], char inputPassword[255],
                       string &action);

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{

  // Create application window
  WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
  ::RegisterClassExW(&wc);
  HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

  // Initialize Direct3D
  if (!CreateDeviceD3D(hwnd)) {
      CleanupDeviceD3D();
      ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
      return 1;
  }

  // Show the window
  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  io.Fonts->Clear();
  ImFont* font = io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", 45.0f);
  ImFont* errorFont = io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", 20.0f);
  ImFont* transactionsListFont = io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", 30.0f);
  io.Fonts->Build();
  //ImGui::StyleColorsLight();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX9_Init(g_pd3dDevice);

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  curl = curl_easy_init();
  if (!curl) {
      cout << "ERROR: Failed to initialize CURL." << endl;
      return 1;
  }
  
  thread messageThread(refreshAndPrintPriceEvery60s);
  // Main loop
  char inputLogin[255] = {}; 
  char inputPassword[255] = {};
  char inputFirstName[255] = {}; 
  char inputLastName[255] = {};

  string inputLoginError;
  string inputPasswordError;
  string inputFirstNameError;
  string inputLastNameError;

  string action = "chooseLoginOrRegister";

  string buyCrypto;
  int buyCryptoError;
  string buyCryptoErrorMsg = "";
  double cryptoAmountToBuy = 0;

  string sellCrypto;
  int sellCryptoError;
  string sellCryptoErrorMsg = "";
  double cryptoAmountToSell = 0;

  bool depositMoney = false;
  int fiatToDeposit = 0;
  int depositMoneyError = 0;
  string depositMoneyErrorMsg = "";
  
  bool done = false;
  while (!done)
  {
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT) done = true;
    }
    if (done)
      break;

    // Handle lost D3D9 device
    if (g_DeviceLost)
    {
      HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
      if (hr == D3DERR_DEVICELOST)
      {
          ::Sleep(10);
          continue;
      }
      if (hr == D3DERR_DEVICENOTRESET)
          ResetDevice();
      g_DeviceLost = false;
    }

    // Handle window resize (we don't resize directly in the WM_SIZE handler)
    if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
    {
      g_d3dpp.BackBufferWidth = g_ResizeWidth;
      g_d3dpp.BackBufferHeight = g_ResizeHeight;
      g_ResizeWidth = g_ResizeHeight = 0;
      ResetDevice();
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

/////////////////////////
    // ChooseRegsterOrLogin Window
    if (user.getUserLoginStatus() == false && action == "chooseLoginOrRegister") {
      renderChooseRegsterOrLoginWindow(action);
    }

    // Register window
    if (user.getUserLoginStatus() == false && action == "register") {
      renderRegisterWindow(inputLogin, inputPassword, inputFirstName, inputLastName,
                            inputLoginError, inputPasswordError, inputFirstNameError, inputLastNameError,
                            errorFont, action);
    }
    // Login window
    if (user.getUserLoginStatus() == false && action == "login") {
      renderLoginWindow(inputLogin, inputPassword, action);
    }


    if (user.getUserLoginStatus() == true) {
      float containerWidth = 1280.0f;
      float containerHeight = 1080.0f;
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));

      // Okno główne z możliwością przewijania
      ImGui::Begin("Crypto list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

      for (int i = 0; i < crypto.numberOfCrypto; i++)
      {
        // Wymiary rzędu kryptowaluty
        ImVec2 rowSize(containerWidth, 100.0f);

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

    ///////////////////////

      containerWidth = 640.0f;
      containerHeight = 489.0f;

      ImGui::SetNextWindowPos(ImVec2(1280, 0));
      ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));



      ImGui::Begin("Wallet", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
      ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight));

      vector<vector<string>> wallet;
      {
        std::lock_guard<std::mutex> lock(db_mutex);
        wallet = user.returnAllRecordsFromWallet();
      }

      ImVec2 rowSize(containerWidth, 40.0f);
      ImGui::BeginChild("WalletHeader", rowSize, false, ImGuiWindowFlags_NoScrollbar);
      ImGui::SetCursorPos(ImVec2(264, 0));
      ImGui::Text("Wallet");
      ImGui::EndChild();
      ImGui::Spacing();

      //ImVec2 rowSize(containerWidth, 40.0f);
      ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
      ImGui::PushFont(transactionsListFont);
      ImGui::BeginChild("WalletMainInfo", ImVec2(containerWidth, 80.0f), false, ImGuiWindowFlags_NoScrollbar);
      ImGui::SetCursorPos(ImVec2(10, 6));
      ImGui::SetCursorPos(ImVec2(10, 6));
      std::ostringstream streamValue1;
      streamValue1 << std::fixed << std::setprecision(2) << user.getBalanceInCents() / 100.;
      ImGui::Text(("Fiat balance: " + streamValue1.str() + "$").c_str()); // Fiat balance
      ImGui::SetCursorPos(ImVec2(10, 6 + 33));

      int totalValueCent = 0;
      for (auto v : wallet) {
        totalValueCent += stod(v[2]);
      }
      totalValueCent += user.getBalanceInCents();
      std::ostringstream streamValue2;
      streamValue2 << std::fixed << std::setprecision(2) << totalValueCent / 100.;
      ImGui::Text(("Wallet value: " + streamValue2.str() + "$").c_str());  // Wallet value in $
      ImGui::PopFont();
      ImGui::PopStyleColor();

      ImGui::PushFont(transactionsListFont);
      ImGui::SetCursorPos(ImVec2(410, 10));
      if (ImGui::Button("Deposit money", ImVec2(200, 60))) {
        depositMoney = true;
      }
      ImGui::PopFont();
      

      ImGui::EndChild();
      ImGui::Spacing();

      for (auto record : wallet)
      {
        // Wymiary rzędu kryptowaluty
        ImVec2 rowSize(containerWidth, 80.0f);

        // Tło rzędu (opcjonalne)
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::BeginChild((std::string("Transaction_") + std::string(record[0])).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(transactionsListFont);

        std::ostringstream streamValue;
        streamValue << std::fixed << std::setprecision(2) << stoi(record[2]) / 100.0;

        float textLineSize = 33.0f;
        float textColumnSize = 310.0f;

        auto firstLetterToUpperCase = [](string text) -> string {
          text[0] -= 32;
          return text;
        };

        ImGui::SetCursorPos(ImVec2(10, 6));
        ImGui::Text((firstLetterToUpperCase(record[1])).c_str()); // Crypto name
        ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize));
        ImGui::Text(("Amount: " + record[3]).c_str()); // Amount

        ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6));
        ImGui::Text(("Value: " + streamValue.str() + "$").c_str());  // Value in $
        ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6 + textLineSize));
        ImGui::Text(("Price: " + record[4] + "$").c_str());  // Price of crypto
        ImGui::PopFont();
        ImGui::EndChild();
        ImGui::PopStyleColor();

        // Dodanie odstępu między rzędami
        ImGui::Spacing();
      }

      ImGui::End();


    
    ///////////////////

      containerWidth = 640.0f;
      containerHeight = 591.0f;

      ImGui::SetNextWindowPos(ImVec2(1280, 489));
      ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));



      ImGui::Begin("Transactions list", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
      ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight));

      vector<vector<string>> transactionsList;
      {
        std::lock_guard<std::mutex> lock(db_mutex);
        transactionsList = user.returnAllTransactions();
      }

      //ImVec2 rowSize(containerWidth, 40.0f);
      ImGui::SetCursorPos(ImVec2(133, 5));
      ImGui::BeginChild("TransactionsHistory", rowSize, false, ImGuiWindowFlags_NoScrollbar);
      ImGui::Text("Transactions History");
      ImGui::EndChild();
      ImGui::Spacing();

      for (auto transaction : transactionsList)
      {
        // Wymiary rzędu kryptowaluty
        ImVec2 rowSize(containerWidth, 110.0f);

        // Tło rzędu (opcjonalne)
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::BeginChild((std::string("Transaction_") + std::string(transaction[0])).c_str(), rowSize, false, ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(transactionsListFont);

        std::ostringstream streamValue;
        streamValue << std::fixed << std::setprecision(2) << stoi(transaction[5]) / 100.0;

        float textLineSize = 33.0f;
        float textColumnSize = 310.0f;

        auto firstLetterToUpperCase = [](string text) -> string {
          text[0] -= 32;
          return text;
        };
        auto allLettersToUpperCase = [](string text) -> string {
          for (int i = 0; i < text.size(); i++) {
            text[i] -= 32;
          }
          return text;
        };

        ImGui::SetCursorPos(ImVec2(10, 6));
        ImGui::Text((allLettersToUpperCase(transaction[6]) + " " + firstLetterToUpperCase(transaction[2])).c_str()); // Type + crypto name
        ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize));
        ImGui::Text(("Amount: " + transaction[3]).c_str()); // Amount
        ImGui::SetCursorPos(ImVec2(10, 6 + textLineSize * 2));
        ImGui::Text(("Value: " + streamValue.str() + "$").c_str()); // Value in $

        ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6));
        ImGui::Text((transaction[1]).c_str());  // Date of transaction
        ImGui::SetCursorPos(ImVec2(10 + textColumnSize, 6 + textLineSize));
        ImGui::Text(("Price: " + transaction[4] + "$").c_str());  // Price of crypto
        ImGui::PopFont();
        ImGui::EndChild();
        ImGui::PopStyleColor();

        // Dodanie odstępu między rzędami
        ImGui::Spacing();
      }

      ImGui::End();

      if (buyCrypto != "") {
          containerWidth = 600.0f;
          containerHeight = 300.0f;

          ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
          ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
          ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
          ImGui::SetNextWindowFocus();
          ImGui::Begin("BuyCrypto", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
          ImGui::Text(("Buy " + buyCrypto).c_str());
          ImGui::SetCursorPos(ImVec2(540, 0));
          if (ImGui::Button("X", ImVec2(60, 60))) {
            buyCrypto = "";
            cryptoAmountToBuy = 0;
          }
          ImGui::Spacing();
          ImGui::InputDouble("##AmountToSpend", &cryptoAmountToBuy, 0.1, 1.0, "%.4f");
          cryptoAmountToBuy = std::max(0.0, cryptoAmountToBuy);
          if (cryptoAmountToBuy < 0.0001) cryptoAmountToBuy = 0.0;
          int valueCent = 0;
          if (crypto.getCryptoPrice(buyCrypto).empty() == false && cryptoAmountToBuy != 0.0) {
            valueCent = cryptoAmountToBuy * 100 * stod(crypto.getCryptoPrice(buyCrypto)) + 1;
          }
          std::ostringstream ss_valueDolars;
          if (cryptoAmountToBuy != 0.0) {
            ss_valueDolars << std::fixed << std::setprecision(2) << valueCent / 100.;
          } else {
            ss_valueDolars << 0;
          }
          
          ImGui::Text(("Value " + (ss_valueDolars).str() + "$").c_str());
          ImGui::PushFont(errorFont);
          ImGui::Text((buyCryptoErrorMsg).c_str());
          ImGui::PopFont();
          if (ImGui::Button("BUY", ImVec2(200, 80))) {
            buyCryptoError = user.buyCrypto(crypto.getCryptoId(buyCrypto), cryptoAmountToBuy, valueCent);
            if (buyCryptoError == 0) {
              buyCrypto = "";
              cryptoAmountToBuy = 0;
              buyCryptoErrorMsg = "";
            } else if (buyCryptoError == 1) {
              buyCryptoErrorMsg = "You can't afford it.";
            } else if (buyCryptoError == 2) {
              buyCryptoErrorMsg = "Enter amount of crypto.";
            } else {
              buyCryptoErrorMsg = "Undefined error.";
            }

          }
          ImGui::PopStyleColor();
          ImGui::End();
      }

      if (sellCrypto != "") {
          containerWidth = 600.0f;
          containerHeight = 300.0f;

          ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
          ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
          ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
          ImGui::SetNextWindowFocus();
          ImGui::Begin("SellCrypto", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
          ImGui::Text(("Sell " + sellCrypto).c_str());
          ImGui::SetCursorPos(ImVec2(540, 0));
          if (ImGui::Button("X", ImVec2(60, 60))) {
            sellCrypto = "";
            cryptoAmountToSell = 0;
          }
          ImGui::Spacing();
          ImGui::InputDouble("##AmountToSpend", &cryptoAmountToSell, 0.1, 1.0, "%.4f");
          cryptoAmountToSell = std::max(0.0, cryptoAmountToSell);
          if (cryptoAmountToSell < 0.0001) cryptoAmountToSell = 0.0;
          int valueCent = 0;
          if (crypto.getCryptoPrice(sellCrypto).empty() == false) {
            valueCent = cryptoAmountToSell * 100 * stod(crypto.getCryptoPrice(sellCrypto)) - 1;
          }
          std::ostringstream ss_valueDolars;
          if (cryptoAmountToSell != 0) {
            ss_valueDolars << std::fixed << std::setprecision(2) << valueCent / 100.;
          } else {
            ss_valueDolars << 0;
          }
          
          ImGui::Text(("Value " + (ss_valueDolars).str() + "$").c_str());
          ImGui::PushFont(errorFont);
          ImGui::Text((sellCryptoErrorMsg).c_str());
          ImGui::PopFont();
          if (ImGui::Button("SELL", ImVec2(200, 80))) {
            sellCryptoError = user.sellCrypto(crypto.getCryptoId(sellCrypto), cryptoAmountToSell, valueCent);
            if (sellCryptoError == 0) {
              sellCrypto = "";
              cryptoAmountToSell = 0;
              sellCryptoErrorMsg = "";
            } else if (sellCryptoError == 1) {
              sellCryptoErrorMsg = "Enter amount of crypto.";
            } else if (sellCryptoError == 2) {
              sellCryptoErrorMsg = "You don't have enough crypto in wallet.";
            } else {
              sellCryptoErrorMsg = "Undefined error.";
            }
          }
          ImGui::PopStyleColor();
          ImGui::End();
      }


      if (depositMoney == true) {
          containerWidth = 600.0f;
          containerHeight = 300.0f;

          ImGui::SetNextWindowPos(ImVec2(1920 / 2 - 300, 1080 / 2 - 150));
          ImGui::SetNextWindowSize(ImVec2(containerWidth, containerHeight));
          ImGui::SetWindowSize(ImVec2(containerWidth, containerHeight)); 
          ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
          ImGui::SetNextWindowFocus();
          ImGui::Begin("DepositMoney", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
          ImGui::Text("Deposit money");
          ImGui::SetCursorPos(ImVec2(540, 0));
          if (ImGui::Button("X", ImVec2(60, 60))) {
            depositMoney = false;
            fiatToDeposit = 0;
          }
          ImGui::Spacing();
          ImGui::InputInt("##InputMoneyToDeposit", &fiatToDeposit, 100, 1000);

          ImGui::PushFont(errorFont);
          ImGui::Text((depositMoneyErrorMsg).c_str());
          ImGui::PopFont();

          if (ImGui::Button("DEPOSIT", ImVec2(200, 80))) {
            fiatToDeposit = std::max(0, fiatToDeposit);
            int fiatToDepositCent = fiatToDeposit * 100;
            
            depositMoneyError = user.deposit(fiatToDepositCent);
            
            if (depositMoneyError == 0) {
              depositMoneyErrorMsg = "";
              depositMoney = false;
            } else if (depositMoneyError == 1 || depositMoneyError == 3) {
              depositMoneyErrorMsg = "Max fiat balance is 1 000 000$.";
            } else if (depositMoneyError == 2) {
              depositMoneyErrorMsg = "Enter value from 1 - 1 000 000$.";
            }
            fiatToDeposit = 0;

          }
          ImGui::PopStyleColor();
          ImGui::End();
      }
    }


    // Rendering
    ImGui::EndFrame();
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
    g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    if (g_pd3dDevice->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        g_pd3dDevice->EndScene();
    }

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST) g_DeviceLost = true;
  }

  // Cleanup
  ImGui_ImplDX9_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(hwnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

  return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr) return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) return false;

    return true;
}

void CleanupDeviceD3D()
{
  if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
  if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
      IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
      return true;

    switch (msg)
    {
    case WM_SIZE:
      if (wParam == SIZE_MINIMIZED)
        return 0;
      g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
      g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
      if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
        return 0;
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
    case WM_DPICHANGED:
      if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
      {
        //const int dpi = HIWORD(wParam);
        //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
        const RECT* suggested_rect = (RECT*)lParam;
        ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
      }
      break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}



void renderChooseRegsterOrLoginWindow(string &action) {
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


void renderRegisterWindow(char inputLogin[255], char inputPassword[255], char inputFirstName[255], char inputLastName[255],
                          string &inputLoginError, string &inputPasswordError, string &inputFirstNameError, string &inputLastNameError,
                          ImFont* errorFont,
                          string &action) {
  ImGui::SetNextWindowPos(ImVec2(660, 190));
  ImGui::SetNextWindowSize(ImVec2(600, 650));

  ImGui::Begin("Register", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.

  ImGui::SetCursorPos(ImVec2(20, 20));
  ImGui::Text("Login:");

  ImGui::SetCursorPos(ImVec2(20, 70));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##logininput", inputLogin, CHAR_MAX);

  if (inputLoginError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 125));
    ImGui::Text(inputLoginError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(20, 150));
  ImGui::Text("Password:");

  ImGui::SetCursorPos(ImVec2(20, 200));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##passwordinput", inputPassword, CHAR_MAX, ImGuiInputTextFlags_Password);

  if (inputPasswordError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 255));
    ImGui::Text(inputPasswordError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(20, 300));
  ImGui::Text("First name:");

  ImGui::SetCursorPos(ImVec2(20, 350));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##firstnameinput", inputFirstName, CHAR_MAX);

  if (inputFirstNameError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 405));
    ImGui::Text(inputFirstNameError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(20, 430));
  ImGui::Text("Last name:");

  ImGui::SetCursorPos(ImVec2(20, 480));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##lastnameinput", inputLastName, CHAR_MAX);

  if (inputLastNameError != "") {
    ImGui::PushFont(errorFont);
    ImGui::SetCursorPos(ImVec2(30, 535));
    ImGui::Text(inputLastNameError.c_str());
    ImGui::PopFont();
  } 


  ImGui::SetCursorPos(ImVec2(200, 570));
  if (ImGui::Button("Sign in", ImVec2(200, 60))) {
    int errorCode = user.registerUser(inputLogin, inputPassword, inputFirstName, inputLastName);
    inputLoginError = "";
    inputPasswordError = "";
    inputFirstNameError = "";
    inputLastNameError = "";

    if (errorCode != 0) {
      if (errorCode < 10) {
        if (errorCode == 1) inputLoginError = "Login must be between 8 and 32 characters.";
        else if (errorCode == 2) inputLoginError = "Login must contain only letters.";
        else if (errorCode == 3) inputLoginError = "Account with this login already exists.";
        else inputLoginError = "Wrong login.";
        inputLogin[0] = '\0'; 

      } else if (errorCode < 100) {
        errorCode %= 10;
        if (errorCode == 1) inputPasswordError = "Password must be between 8 and 32 characters.";
        else if (errorCode == 2) inputPasswordError = "Password must contain only lower, capital letters,\ndigits and special characters.";
        else if (errorCode == 3) inputPasswordError = "Password must contain at least 4 lower letters, 1 capital letter,\n2 digits and 1 special character.";
        else inputPasswordError = "Wrong password.";
        inputPassword[0] = '\0';
      } else if (errorCode < 1000) {
        inputFirstNameError = "Enter first name.";
        inputFirstName[0] = '\0'; 

      } else {
        inputLastNameError = "Enter last name.";
        inputLastName[0] = '\0';
      }
    }
  }

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    action = "chooseLoginOrRegister";
  }

  ImGui::End();
}


void renderLoginWindow(char inputLogin[255], char inputPassword[255],
                       string &action) {
  ImGui::SetNextWindowPos(ImVec2(660, 290));
  ImGui::SetNextWindowSize(ImVec2(600, 400));

  ImGui::Begin("Login", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.

  ImGui::SetCursorPos(ImVec2(20, 20));
  ImGui::Text("Login:");
  
  ImGui::SetCursorPos(ImVec2(20, 80));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##logininput", inputLogin, CHAR_MAX);
  
  ImGui::SetCursorPos(ImVec2(20, 150));
  ImGui::Text("Password:");

  ImGui::SetCursorPos(ImVec2(20, 210));
  ImGui::SetNextItemWidth(560);
  ImGui::InputText("##passwordinput", inputPassword, CHAR_MAX, ImGuiInputTextFlags_Password);

  ImGui::SetCursorPos(ImVec2(200, 300));
  // ImGui::SetNextItemWidth(200);
  if (ImGui::Button("Login", ImVec2(200, 60))) {
    user.loginUser(inputLogin, inputPassword);
    if (user.getUserLoginStatus() == false) {
      inputLogin[0] = '\0'; 
      inputPassword[0] = '\0';
    }
  }

  ImGui::SetCursorPos(ImVec2(540, 0));
  if (ImGui::Button("X", ImVec2(60, 60))) {
    action = "chooseLoginOrRegister";
  }


  ImGui::End();
  
}