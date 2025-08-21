#include <iostream>
#include <string>

// Custom scripts
#include "scripts/get_data.h"
#include "scripts/pitch_select.h"

int main() {
    std::string pitcher_first_name;
    std::string pitcher_last_name;
    std::string pitcher_year;
    std::string batter_first_name;
    std::string batter_last_name;
    std::string batter_year;

    std::cout << "Enter pitcher first name: ";
    std::cin >> pitcher_first_name;
    std::cout << "Enter pitcher last name: ";
    std::cin >> pitcher_last_name;
    std::cout << "Enter year for stats: ";
    std::cin >> pitcher_year;

    std::cout << "Enter batter first name: ";
    std::cin >> batter_first_name;
    std::cout << "Enter batter last name: ";
    std::cin >> batter_last_name;
    std::cout << "Enter year for stats: ";
    std::cin >> batter_year;

    Pitcher pitcher(pitcher_first_name, pitcher_last_name, pitcher_year);
    Batter batter(batter_first_name, batter_last_name, batter_year);

    bool pitcher_found = read_pitcher_db(pitcher);
    bool batter_found = read_batter_db(batter);
    
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
    }

    std::cout << "Pitcher found: " << pitcher.get_first_name() << " " << pitcher.get_last_name() << ", ID = " << pitcher.get_id() << ", Hand = " << pitcher.get_hand() << std::endl;
    std::cout << "Pitch Mix In " << pitcher.get_year() << ": " << pitcher.get_pitch_mix() << std::endl;
    std::cout << "Usage Vs. Righty In 0-0 Count: " << pitcher.get_usage_vs_righty()["0"]["0"] << std::endl;
    std::cout << "Usage Vs. Lefty In 3-2 Count: " << pitcher.get_usage_vs_lefty()["3"]["2"] << std::endl;

    std::cout << "Batter found: " << pitcher.get_first_name() << " " << pitcher.get_last_name() << ", ID = " << pitcher.get_id() << ", Hand = " << pitcher.get_hand() << std::endl;
    std::cout << "Usage Vs. Righty In 0-0 Count: " << pitcher.get_usage_vs_righty()["0"]["0"] << std::endl;
    std::cout << "Usage Vs. Lefty In 3-2 Count: " << pitcher.get_usage_vs_lefty()["3"]["2"] << std::endl;
    return 0;
}