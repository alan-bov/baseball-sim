// scripts/at_bat.h
#ifndef AT_BAT_H
#define AT_BAT_H

#include <string>
#include "get_data.h"

class At_Bat {
    private:
        bool outcome;
        std::string generated_outcome;
        int balls;
        int strikes;
        Pitcher pitcher;
        Batter batter;
    public:
        At_Bat(Pitcher pitcher_in, Batter batter_in);
        int get_balls() const;
        int get_strikes() const;
        void increment_balls();
        void increment_strikes();
        void sim_pitch(json& pitcher_stats, json& batter_stats);
        std::string sim_at_bat();
        std::string generate_outcome();
};

#endif
