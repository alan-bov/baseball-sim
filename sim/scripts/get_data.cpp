#include "get_data.h"

#include <string>
#include <iostream>
#include <cstdlib>

// PITCHER METHODS
Pitcher::Pitcher(std::string fn, std::string ln, std::string y)
    : first_name(fn), last_name(ln), year(y) {}

void Pitcher::set_id(int id) {
    player_id = id;
}

void Pitcher::set_hand(std::string h) {
    hand = h;
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

// HELPER FUNCTIONS


// FUNCTIONS FOR USE ELSEWHERE
Pitcher read_pitcher_db(Pitcher pitcher) {
    try {
        return pitcher;
    }
    catch (...) {
        std::cout << "Error reading from data/pitchers.db." << std::endl;
        return pitcher;
    }
}

Batter read_batter_db(Batter batter) {
    try {
        return batter;
    }
    catch (...) {
        std::cout << "Error reading from data/batters.db." << std::endl;
        return batter;
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
