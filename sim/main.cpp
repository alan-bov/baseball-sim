#include <iostream>

// Custom scripts
#include "scripts/get_data.h"
#include "scripts/pitch_select.h"

int main() {
    Pitcher pitcher("Tarik", "Skubal", "2024");
    bool found = read_pitcher_db(pitcher);
    while (!found) {
        std::cout << "Pitcher not found, adding to database..." << std::endl;
        get_pitcher(pitcher.get_first_name(), pitcher.get_last_name(), pitcher.get_year());
        std::cout << "Rereading pitcher database to find new addition." << std::endl;
        found = read_pitcher_db(pitcher);
    }
    std::cout << "Pitcher found: " << pitcher.get_first_name() << " " << pitcher.get_last_name() << ", ID = " << pitcher.get_id() << ", Hand = " << pitcher.get_hand() << std::endl;
    std::cout << "Pitch Mix In " << pitcher.get_year() << ": " << pitcher.get_pitch_mix() << std::endl;
    std::cout << "Changeup Usage Vs. Righty In 0-0 Count: " << pitcher.get_usage_vs_righty()["0"]["0"]["CH"] << std::endl;
    std::cout << "Usage Vs. Lefty In 3-2 Count: " << pitcher.get_usage_vs_lefty()["3"]["2"] << std::endl;
    return 0;
}