#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "at_bat.h"
#include "get_data.h"
#include "pitch_select.h"

using json = nlohmann::json;

// At_Bat class methods
At_Bat::At_Bat(Pitcher pitcher_in, Batter batter_in)
    : outcome(false), balls(0), strikes(0), pitcher(pitcher_in), batter(batter_in) {}

int At_Bat::get_balls() const { return balls; }

int At_Bat::get_strikes() const { return strikes; }

void At_Bat::increment_balls() {
    if (balls < 3) { balls += 1; }
}

void At_Bat::increment_strikes() {
    if (strikes < 2) { strikes += 1; }
}

std::string At_Bat::sim_pitch(json& pitcher_stats, json& batter_stats) {
    // Select pitch
    std::string selected_pitch = select_pitch(std::to_string(balls), std::to_string(strikes), pitcher.get_pitch_mix(), pitcher_stats, batter_stats);
    double pitch_speed = get_pitch_speed(selected_pitch, pitcher);
    std::cout << "The " << balls << "-" << strikes << " pitch is a " << selected_pitch << " at " << pitch_speed << " mph." << std::endl; 

    // Update probability table based on model
    return "pass";
}

std::string At_Bat::sim_at_bat() {
    json pitcher_stats;
    json batter_stats;
    if (pitcher.get_hand() == "Switch") {
        if (batter.get_hand() == "Left") {
            // Lefty batter, switch pitcher will pitch right
            batter_stats = batter.get_pitches_seen_righty();
        } else {
            // Righty batter, switch pitcher will pitch left
            batter_stats = batter.get_pitches_seen_lefty();
        }
    } else if (pitcher.get_hand() == "Left") {
        batter_stats = batter.get_pitches_seen_lefty();
    } else {
        batter_stats = batter.get_pitches_seen_righty();
    }

    if (batter.get_hand() == "Switch") {
        if (pitcher.get_hand() == "Left") {
            // Lefty pitcher, switch batter will bat right
            pitcher_stats = pitcher.get_usage_vs_righty();
        } else {
            // Righty pitcher, switch batter will bat left
            pitcher_stats = pitcher.get_usage_vs_lefty();
        }
    } else if (batter.get_hand() == "Left") {
        pitcher_stats = pitcher.get_usage_vs_lefty();
    } else {
        pitcher_stats = pitcher.get_usage_vs_righty();
    }

    std::cout << "Simulating at bat between pitcher [" << pitcher.get_year() << " " << pitcher.get_first_name() << " "
            << pitcher.get_last_name() << "] and batter [" << batter.get_year() << " " << batter.get_first_name() << " "
            << batter.get_last_name() << "]." << std::endl;
    int tries = 7;
    while (!outcome) {
        std::string generated_outcome = sim_pitch(pitcher_stats, batter_stats);
        if (outcome || tries < 1) {
            std::cout << "At bat ends with generated outcome of: " << generated_outcome << std::endl;
            return generated_outcome;
        }
        if (tries % 2 == 0) {
            increment_balls();
        } else {
            increment_strikes();
        }
        tries -= 1;
    }
    return "pass";
}

std::string At_Bat::generate_outcome() {
    return "pass";
}
