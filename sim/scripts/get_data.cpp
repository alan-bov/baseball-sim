#include "get_data.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include "sqlite3.h"

// PITCHER METHODS
Pitcher::Pitcher(std::string fn, std::string ln, std::string y)
    : first_name(fn), last_name(ln), year(y) {}

void Pitcher::set_id(int id) {
    player_id = id;
}

void Pitcher::set_hand(std::string h) {
    hand = h;
}

const std::string& Pitcher::get_first_name() const {
    return first_name;
}

const std::string& Pitcher::get_last_name() const {
    return last_name;
}

const std::string& Pitcher::get_year() const {
    return year;
}

const int& Pitcher::get_id() const {
    return player_id;
}

const std::string& Pitcher::get_hand() const {
    return hand;
}

// BATTER METHODS
Batter::Batter(std::string fn, std::string ln, std::string y)
    : first_name(fn), last_name(ln), year(y) {}

void Batter::set_id(int id) {
    player_id = id;
}

void Batter::set_hand(std::string h) {
    hand = h;
}
const std::string& Batter::get_first_name() const {
    return first_name;
}

const std::string& Batter::get_last_name() const {
    return last_name;
}

const std::string& Batter::get_year() const {
    return year;
}

const int& Batter::get_id() const {
    return player_id;
}

const std::string& Batter::get_hand() const {
    return hand;
}

// HELPER FUNCTIONS
sqlite3* init_db(const char* db_path) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        if (db) sqlite3_close(db);
        return nullptr;
    }
    return db;
}

// FUNCTIONS FOR USE ELSEWHERE
bool read_pitcher_db(Pitcher& pitcher) {
    const char* db_path = "..\\data\\pitchers.db";
    sqlite3* db = init_db(db_path);
    if (!db) {
        std::cerr << "Failed to open DB inside read_pitcher_db." << std::endl;
        return false;
    }

    const char* sql = R"(
        SELECT player_id, hand
        FROM pitchers
        WHERE first_name = ? AND last_name = ? AND year = ?
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, pitcher.get_first_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pitcher.get_last_name().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, pitcher.get_year().c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        pitcher.set_id(sqlite3_column_int(stmt, 0));
        const unsigned char* hand_text = sqlite3_column_text(stmt, 1);
        if (hand_text) pitcher.set_hand(reinterpret_cast<const char*>(hand_text));

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;  // Found and updated pitcher
    }

    // Not found or error
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

bool read_batter_db(Batter batter) {
    try {
        return true;
    }
    catch (...) {
        std::cout << "Error reading from data/batters.db." << std::endl;
        return false;
    }
}

bool get_pitcher(std::string first_name, std::string last_name, std::string year) {
    try {
        std::string python_path = R"(..\\collection\\.venv\\Scripts\\python.exe)";
        std::string script_path = R"(..\\collection\\main.py)";
        std::string command = "cmd /c \"\"" + python_path + "\" \"" + script_path + "\" \"" + first_name + "\" \"" + last_name + "\" \"" + year + "\" pitcher\"";
        int ret = system(command.c_str());

        if (ret == 0) {
            return true;
        } else {
            return false;
        }
    }
    catch (...) {
        return false;
    }
}

bool get_batter(std::string first_name, std::string last_name, std::string year) {
    try {
        std::string python_path = R"(..\\collection\\.venv\\Scripts\\python.exe)";
        std::string script_path = R"(..\\collection\\main.py)";
        std::string command = "cmd /c \"\"" + python_path + "\" \"" + script_path + "\" \"" + first_name + "\" \"" + last_name + "\" \"" + year + "\" batter\"";
        int ret = system(command.c_str());

        if (ret == 0) {
            return true;
        } else {
            return false;
        }
    }
    catch (...) {
        return false;
    }
}
