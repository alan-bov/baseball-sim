// scripts/pitch_select.h
#ifndef PITCH_SELECT_H
#define PITCH_SELECT_H

#include <string>
#include <nlohmann/json.hpp>
#include "get_data.h"

using json = nlohmann::json;

const std::string select_pitch(std::string balls, std::string strikes, const json& pitch_mix, const json& pitcher_stats, const json& batter_stats);
std::string choose_pitch_from_table(const json& probability_table);
double get_pitch_speed(const std::string& pitch_type, const Pitcher& pitcher);

#endif