#include <iostream>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>
#include "pitch_select.h"

using json = nlohmann::json;

std::string get_arm_matchup() {
    return "pass";
}

const json select_pitch(std::string balls, std::string strikes, const json& pitch_mix, const json& pitcher_stats, const json& batter_stats) {
    json probability_table;

    // Tuning parameters below. If pitcher_tuner > batter_tuner, pitch selection will favor the pitcher's numbers & vice versa
    double pitcher_tuner = 1;
    double batter_tuner = 1;

    // Isolate stats by count
    json pitcher_count_stats = pitcher_stats[balls][strikes];
    json batter_count_stats = batter_stats[balls][strikes];

    // Create probability table for pitch
    for (const auto& pitch:pitch_mix) {
        // Case 1: pitcher has never thrown pitch in count and batter has never seen pitch in count
        if (!pitcher_count_stats.contains(pitch) && !batter_count_stats.contains(pitch)) {
            probability_table[pitch] = 0;
            std::cout << pitch << " probability is defined by Case 1." << std::endl;
            continue;
        }
        // Case 2: pitcher has thrown pitch in count but batter has never seen pitch in count
        if (pitcher_count_stats.contains(pitch) && !batter_count_stats.contains(pitch)) {
            double pitcher_pitch_probability = std::stod(pitcher_count_stats[pitch].get<std::string>());
            probability_table[pitch] = pow(pitcher_pitch_probability, pitcher_tuner);
            std::cout << pitch << " probability is defined by Case 2." << std::endl;
            continue;
        }
        // Case 3: batter has seen pitch in count but pitcher has never thrown pitch in count
        if (!pitcher_count_stats.contains(pitch) && batter_count_stats.contains(pitch)){
            double batter_pitch_probability = std::stod(batter_count_stats[pitch].get<std::string>());
            double pitcher_pitch_probability = 0.1 * batter_pitch_probability;
            probability_table[pitch] = pow(pitcher_pitch_probability, pitcher_tuner) * pow(1-batter_pitch_probability, batter_tuner);
            std::cout << pitch << " probability is defined by Case 3." << std::endl;
            continue;
        }
        //Case 4: pitcher has thrown pitch in count and batter has seen pitch in count
        if (pitcher_count_stats.contains(pitch) && batter_count_stats.contains(pitch)) {
            double pitcher_pitch_probability = std::stod(pitcher_count_stats[pitch].get<std::string>());
            double batter_pitch_probability = std::stod(batter_count_stats[pitch].get<std::string>());
            probability_table[pitch] = pow(pitcher_pitch_probability, pitcher_tuner) * pow(1-batter_pitch_probability, batter_tuner);
            std::cout << pitch << " probability is defined by Case 4." << std::endl;
            continue;
        }
        std::cerr << "ERROR: No case found for " << pitch << " in " << balls << "-" << strikes << " count!" << std::endl;
    }

    // Normalize probabilities to sum to 1
    double sum_probs = 0.0;
    for (auto& [pitch, value] : probability_table.items()) {
        sum_probs += value.get<double>();
    }

    if (sum_probs > 0) {
        for (auto& [pitch, value] : probability_table.items()) {
            value = value.get<double>() / sum_probs;
        }
    } else {
        // fallback: if all probabilities are 0, assign uniform distribution
        double uniform = 1.0 / pitch_mix.size();
        for (const auto& pitch : pitch_mix) {
            probability_table[pitch] = uniform;
        }
    }

    std::cout << "Pitch probability table: " << probability_table << std::endl;

    return probability_table;
}
