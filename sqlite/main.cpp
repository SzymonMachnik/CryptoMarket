#include <iostream>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    char *errMsg = nullptr;

    // Otwarcie połączenia z bazą danych
    if (sqlite3_open("../sqlite/database.db", &db)) {
        std::cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Zapytanie SQL - wstawienie danych
    const char *sql = "INSERT INTO users (first_name, last_name) VALUES ('Szymon', 'Machnik');";

    // Wykonanie zapytania SQL
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Błąd podczas wykonywania zapytania: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Dane zostały dodane pomyślnie!" << std::endl;
    }

    // Zamknięcie połączenia z bazą danych
    sqlite3_close(db);

    return 0;
}