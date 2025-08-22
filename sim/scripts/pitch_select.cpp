#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <nlohmann/json.hpp>
#include "pitch_select.h"

using json = nlohmann::json;

const std::string select_pitch(std::string balls, std::string strikes, const json& pitch_mix, const json& pitcher_stats, const json& batter_stats) {
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
            double pitcher_pitch_probability = pitcher_count_stats.at(pitch).get<double>();
            probability_table[pitch] = pow(pitcher_pitch_probability, pitcher_tuner);
            std::cout << pitch << " probability is defined by Case 2." << std::endl;
            continue;
        }
        // Case 3: batter has seen pitch in count but pitcher has never thrown pitch in count
        if (!pitcher_count_stats.contains(pitch) && batter_count_stats.contains(pitch)){
            double batter_pitch_probability = batter_count_stats.at(pitch).get<double>();
            double pitcher_pitch_probability = 0.1 * batter_pitch_probability;
            probability_table[pitch] = pow(pitcher_pitch_probability, pitcher_tuner) * pow(1-batter_pitch_probability, batter_tuner);
            std::cout << pitch << " probability is defined by Case 3." << std::endl;
            continue;
        }
        //Case 4: pitcher has thrown pitch in count and batter has seen pitch in count
        if (pitcher_count_stats.contains(pitch) && batter_count_stats.contains(pitch)) {
            double pitcher_pitch_probability = pitcher_count_stats.at(pitch).get<double>();
            double batter_pitch_probability = batter_count_stats.at(pitch).get<double>();
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

    std::string pitch_selection = choose_pitch_from_table(probability_table);

    return pitch_selection;
}

std::string choose_pitch_from_table(const json& probability_table) {
    // Use a random device + generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dist(0.0, 1.0);

    double r = dist(gen);  // random in [0,1)
    double cumulative = 0.0;

    for (auto& [pitch, prob_json] : probability_table.items()) {
        double prob = prob_json.get<double>();
        cumulative += prob;
        if (r <= cumulative) {
            std::cout << pitch << " selected from list from generated probability " << r << std::endl;
            return pitch;
        }
    }

    // Edge case: floating-point precision → return last pitch
    std::cout << probability_table.begin().key() << " selected from list from generated probability " << r << std::endl;
    return probability_table.begin().key();
}
