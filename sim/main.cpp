#include <iostream>

// Custom scripts
#include "scripts/get_data.h"
#include "scripts/pitch_select.h"

int main() {
    bool p_success = get_pitcher("Tarik", "Skubal", "2024");
    bool b_success = get_batter("Jose", "Ramirez", "2024");
    if (p_success && b_success) {
        std::cout << "New player(s) successfully added to database." << std::endl;
    } else {
        std::cout << "Failed to enter new player into database." << std::endl;
    }
    return 0;
}