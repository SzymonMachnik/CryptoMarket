cmake_minimum_required(VERSION 3.10)

project(CryptoScraper)

# Określenie standardu C++
set(CMAKE_CXX_STANDARD 17)

# Dodanie katalogów z plikami nagłówkowymi
include_directories(
    src
    libs/imgui
    ${CMAKE_SOURCE_DIR}/libs/curl/include
    libs/sqliteLib  # Ścieżka do sqlite3.h
)

link_directories(${CMAKE_SOURCE_DIR}/libs/curl/lib)

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

target_link_libraries(CryptoScraper
    d3d9
    dwmapi
    gdi32
    libcurl
    ssl
    crypto
    ws2_32
    winmm
)

# Kopiowanie czcionki (jeśli potrzebne)
file(COPY ${CMAKE_SOURCE_DIR}/fonts/Roboto-Medium.ttf DESTINATION ${CMAKE_BINARY_DIR}/fonts)
file(COPY ${CMAKE_SOURCE_DIR}/src/crypto/listOfCryptoApiIds.txt DESTINATION ${CMAKE_BINARY_DIR}/src/crypto)
file(COPY ${CMAKE_SOURCE_DIR}/src/crypto/listOfCryptoNames.txt DESTINATION ${CMAKE_BINARY_DIR}/src/crypto)
file(COPY ${CMAKE_SOURCE_DIR}/sqlite/database.db DESTINATION ${CMAKE_BINARY_DIR}/sqlite)