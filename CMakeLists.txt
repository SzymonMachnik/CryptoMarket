cmake_minimum_required(VERSION 3.10)

project(CryptoScraper)

# Określenie standardu C++
set(CMAKE_CXX_STANDARD 11)

# Dodanie katalogów z plikami nagłówkowymi
include_directories(
    src
    libs/curl/include  # Ścieżka do nagłówków curl
    libs/imgui
    libs/sqliteLib  # Ścieżka do sqlite3.h
)

# Źródła projektu
file(GLOB SOURCES
    src/main.cpp
    src/crypto/crypto.cpp
    src/memory/memory.cpp
    src/user/user.cpp
    src/handleGui/handlegui.cpp
    libs/imgui/*.cpp
    libs/sqliteLib/sqlite3.c
)

# Określenie pliku wykonywalnego
add_executable(CryptoScraper ${SOURCES})

# Linkowanie z biblioteką curl (libcurl.a)
target_link_libraries(CryptoScraper
    curl
    d3d9
    dwmapi
    gdi32
)

# Kopiowanie czcionki (jeśli potrzebne)
file(COPY ${CMAKE_SOURCE_DIR}/fonts/Roboto-Medium.ttf DESTINATION ${CMAKE_BINARY_DIR}/fonts)