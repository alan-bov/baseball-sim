#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

// Custom scripts
#include "scripts/get_data.h"
#include "scripts/pitch_select.h"

using json = nlohmann::json;

int main() {
    std::string pitcher_first_name;
    std::string pitcher_last_name;
    std::string pitcher_year;
    std::string batter_first_name;
    std::string batter_last_name;
    std::string batter_year;
    std::string balls;
    int balls_int;
    std::string strikes;
    int strikes_int;

    std::cout << "Enter ONLY pitcher first name: ";
    std::cin >> pitcher_first_name;
    std::cout << "Enter ONLY pitcher last name: ";
    std::cin >> pitcher_last_name;
    std::cout << "Enter year for stats: ";
    std::cin >> pitcher_year;

    std::cout << "Enter ONLY batter first name: ";
    std::cin >> batter_first_name;
    std::cout << "Enter ONLY batter last name: ";
    std::cin >> batter_last_name;
    std::cout << "Enter year for stats: ";
    std::cin >> batter_year;

    while (true) {
        std::cout << "Enter ball count: ";
        std::cin >> balls_int;
        if (std::cin.fail() || balls_int < 0 || balls_int > 3) {
            std::cin.clear(); // reset fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
            std::cout << "Invalid input. Please enter a number between 0 and 3.\n";
        } else {
            balls = std::to_string(balls_int);
            break;
        }
    }

    while (true) {
        std::cout << "Enter strike count: ";
        std::cin >> strikes_int;
        if (std::cin.fail() || strikes_int < 0 || strikes_int > 2) {
            std::cin.clear(); // reset fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
            std::cout << "Invalid input. Please enter a number between 0 and 2.\n";
        } else {
            strikes = std::to_string(strikes_int);
            break;
        }
    }

    Pitcher pitcher(pitcher_first_name, pitcher_last_name, pitcher_year);
    Batter batter(batter_first_name, batter_last_name, batter_year);

    bool pitcher_found = read_pitcher_db(pitcher);
    bool batter_found = read_batter_db(batter);

    int tries = 5;
    
    while (!pitcher_found || !batter_found) {
        if (!pitcher_found) {
            std::cout << "Pitcher not found, adding to database..." << std::endl;
            get_pitcher(pitcher.get_first_name(), pitcher.get_last_name(), pitcher.get_year());
            std::cout << "Rereading pitcher database to find new addition." << std::endl;
            pitcher_found = read_pitcher_db(pitcher);
        }

        if (!batter_found) {
            std::cout << "Batter not found, adding to database..." << std::endl;
            get_batter(batter.get_first_name(), batter.get_last_name(), batter.get_year());
            std::cout << "Rereading batter database to find new addition." << std::endl;
            batter_found = read_batter_db(batter);
        }

        tries -= 1;
        if (tries < 1) {
            if (!pitcher_found) {
                std::cerr << "Pitcher not found: " << pitcher.get_first_name() << " " << pitcher.get_last_name() << " - " << pitcher.get_year() << std::endl;
                std::cerr << "Aborting program..." << std::endl;
                return 0;
            } else {
                std::cerr << "Batter not found: " << batter.get_first_name() << " " << batter.get_last_name() << " - " << batter.get_year() << std::endl;
                std::cerr << "Aborting program..." << std::endl;
                return 0;
            }
        }
    }

    std::cout << "Pitcher found: " << pitcher.get_first_name() << " " << pitcher.get_last_name() << ", ID = " << pitcher.get_id() << ", Hand = " << pitcher.get_hand() << std::endl;
    std::cout << "Batter found: " << batter.get_first_name() << " " << batter.get_last_name() << ", ID = " << batter.get_id() << ", Hand = " << batter.get_hand() << std::endl;

    json pitcher_stats;
    json batter_stats;
    if (pitcher.get_hand() == "Left") {
        batter_stats = batter.get_pitches_seen_lefty();
    } else {
        batter_stats = batter.get_pitches_seen_righty();
    }
    if (batter.get_hand() == "Left") {
        pitcher_stats = pitcher.get_usage_vs_lefty();
    } else {
        pitcher_stats = pitcher.get_usage_vs_righty();
    }

    json probability_table = select_pitch(balls, strikes, pitcher.get_pitch_mix(), pitcher_stats, batter_stats);
    std::string pitch_type = choose_pitch_from_table(probability_table);
    return 0;
}