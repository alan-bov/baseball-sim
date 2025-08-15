#include "get_data.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "sqlite3.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

// PITCHER CLASS METHODS
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
    
const json& Pitcher::get_pitch_mix() const { return pitch_mix; }  
void Pitcher::set_pitch_mix(const std::string& mix_str) {
    pitch_mix = json::parse(mix_str);
}

const json& Pitcher::get_usage_vs_righty() const { return usage_vs_righty; }
void Pitcher::set_usage_vs_righty(const std::string& usage_str) {
    usage_vs_righty = json::parse(usage_str);
}

const json& Pitcher::get_usage_vs_lefty() const { return usage_vs_lefty; }
void Pitcher::set_usage_vs_lefty(const std::string& usage_str) {
    usage_vs_lefty = json::parse(usage_str);
}

// BATTER CLASS METHODS
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
sqlite3* init_db(const fs::path& db_path) {
    sqlite3* db = nullptr;
    // Convert fs::path to UTF-8 string for sqlite3
    int rc = sqlite3_open(db_path.string().c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        if (db) sqlite3_close(db);
        return nullptr;
    }
    return db;
}

fs::path find_data_file(const std::string& filename) {
    // Start in the directory containing the exe
    fs::path current = fs::current_path();

    for (int i = 0; i <= 3; ++i) {
        fs::path candidate = current / "data" / filename;
        std::cout << "Looking for file: " << candidate << std::endl;
        if (fs::exists(candidate)) {
            std::cout << "File found!" << std::endl;
            return candidate;
        }
        // Go one directory up for the next iteration
        current = current.parent_path();
    }
    // Return empty path if not found
    return {};
}

fs::path find_collection_file(const std::string& filename) {
    // Start in the directory containing the exe
    fs::path current = fs::current_path();

    for (int i = 0; i <= 3; ++i) {
        fs::path candidate = current / "collection" / filename;
        std::cout << "Looking for file: " << candidate << std::endl;
        if (fs::exists(candidate)) {
            std::cout << "File found!" << std::endl;
            return candidate;
        }
        // Go one directory up for the next iteration
        current = current.parent_path();
    }
    // Return empty path if not found
    return {};
}

// FUNCTIONS FOR USE ELSEWHERE
bool read_pitcher_db(Pitcher& pitcher) {
    fs::path db_path = find_data_file("pitchers.db");
    if (db_path.empty()) {
        std::cerr << "Error: Could not find data/pitchers.db in current or up to 3 parent directories.\n";
        return false;
    }

    sqlite3* db = init_db(db_path);
    if (!db) {
        std::cerr << "Failed to open DB inside read_pitcher_db." << std::endl;
        return false;
    }

    const char* sql = R"(
        SELECT player_id, hand, pitch_mix, usage_vs_righty, usage_vs_lefty
        FROM pitchers
        WHERE first_name = ? COLLATE NOCASE
          AND last_name = ? COLLATE NOCASE
          AND year = ? COLLATE NOCASE
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

    // Step through the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int player_id = sqlite3_column_int(stmt, 0);
        const unsigned char* hand_text = sqlite3_column_text(stmt, 1);
        const unsigned char* pitch_mix_text = sqlite3_column_text(stmt, 2);
        const unsigned char* usage_vs_righty_text = sqlite3_column_text(stmt, 3);
        const unsigned char* usage_vs_lefty_text = sqlite3_column_text(stmt, 4);

        pitcher.set_id(player_id);
        if (hand_text) pitcher.set_hand(reinterpret_cast<const char*>(hand_text));
        if (pitch_mix_text) {
            try { pitcher.set_pitch_mix(reinterpret_cast<const char*>(pitch_mix_text)); }
            catch (const std::exception& e) {
                std::cerr << "Error parsing pitch_mix JSON: " << e.what() << "\n";
            }
        }

        if (usage_vs_righty_text) {
            try { pitcher.set_usage_vs_righty(reinterpret_cast<const char*>(usage_vs_righty_text)); }
            catch (const std::exception& e) {
                std::cerr << "Error parsing usage_vs_righty JSON: " << e.what() << "\n";
            }
        }

        if (usage_vs_lefty_text) {
            try { pitcher.set_usage_vs_lefty(reinterpret_cast<const char*>(usage_vs_lefty_text)); }
            catch (const std::exception& e) {
                std::cerr << "Error parsing usage_vs_lefty JSON: " << e.what() << "\n";
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    } else if (rc == SQLITE_DONE) {
        std::cout << "DEBUG: Query completed, no matching rows found.\n";
    } else {
        std::cerr << "SQLite step error: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

bool read_batter_db(Batter& batter) {
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
        fs::path python_path = find_collection_file(".venv\\Scripts\\python.exe");
        fs::path script_path = find_collection_file("main.py");
        std::string command = "cmd /c \"\"" + python_path.string() + "\" \"" + script_path.string() + "\" \"" + first_name + "\" \"" + last_name + "\" \"" + year + "\" pitcher\"";
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
