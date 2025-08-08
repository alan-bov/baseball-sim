#include <iostream>

// Custom scripts
#include "scripts/get_data.h"
#include "scripts/pitch_select.h"

int main() {
    Pitcher pitcher("Will", "Vest", "2024");
    bool found = read_pitcher_db(pitcher);
    if (!found) {
        std::cout << "Pitcher not found, adding to database..." << std::endl;
        get_pitcher(pitcher.get_first_name(), pitcher.get_last_name(), pitcher.get_year());  // You implement this function to insert pitcher
    }
    else {
        std::cout << "Pitcher found: ID = " << pitcher.get_id() << ", Hand = " << pitcher.get_hand() << std::endl;
    }
    return 0;
}