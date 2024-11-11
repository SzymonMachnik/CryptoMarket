// #include <iostream>
// #include <sqlite3.h>

// int main() {
//     sqlite3 *db;
//     char *errMsg = nullptr;

//     // Otwarcie połączenia z bazą danych
//     if (sqlite3_open("../sqlite/database.db", &db)) {
//         std::cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << std::endl;
//         return 1;
//     }

//     // Zapytanie SQL - wstawienie danych
//     const char *sql = "INSERT INTO users (first_name, last_name) VALUES ('Szymon', 'Machnik');";

//     // Wykonanie zapytania SQL
//     if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
//         std::cerr << "Błąd podczas wykonywania zapytania: " << errMsg << std::endl;
//         sqlite3_free(errMsg);
//     } else {
//         std::cout << "Dane zostały dodane pomyślnie!" << std::endl;
//     }

//     // Zamknięcie połączenia z bazą danych
//     sqlite3_close(db);

//     return 0;
// }
#include <iostream>
#include <sqlite3.h>
#include <string>

std::string result; // Globalna zmienna do przechowywania wyników

// Funkcja callback
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        // Dodajemy nazwę kolumny i wartość do zmiennej result
        result += std::string(azColName[i]) + ": " + (argv[i] ? argv[i] : "NULL") + " ";
    }
    result += "\n";
    return 0;
}

int main() {
    sqlite3 *db;
    char *errMsg = nullptr;
    const char *sql = "SELECT user_id, first_name FROM users";

    // Otwieramy bazę danych
    if (sqlite3_open("database.db", &db) != SQLITE_OK) {
        std::cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Wykonujemy zapytanie SELECT z funkcją callback
    if (sqlite3_exec(db, sql, callback, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Błąd wykonania zapytania: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // Zamykamy bazę danych
    sqlite3_close(db);

    // Wyświetlamy wynik jako string
    std::cout << "Wynik zapytania:\n" << result << std::endl;

    return 0;
}