// scripts/pitch_select.h
#ifndef PITCH_SELECT_H
#define PITCH_SELECT_H

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string get_arm_matchup();
const json select_pitch(std::string balls, std::string strikes, const json& pitch_mix, const json& pitcher_stats, const json& batter_stats);

#endif