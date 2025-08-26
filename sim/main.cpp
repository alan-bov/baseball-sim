#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

// Custom scripts
#include "scripts/get_data.h"
#include "scripts/pitch_select.h"
#include "scripts/at_bat.h"

using json = nlohmann::json;

int main() {
    std::string pitcher_first_name;
    std::string pitcher_last_name;
    std::string pitcher_year;
    std::string batter_first_name;
    std::string batter_last_name;
    std::string batter_year;

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


    At_Bat at_bat(pitcher, batter);
    std::string outcome = at_bat.sim_at_bat();
}