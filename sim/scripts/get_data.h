// scripts/get_data.h
#ifndef GET_DATA_H
#define GET_DATA_H

#include <string>

class Pitcher {
    private:
        std::string first_name;
        std::string last_name;
        std::string year;
        int player_id;
        std::string hand;

    public:
        Pitcher(std::string fn, std::string ln, std::string y);
        void set_id(int id);
        void set_hand(std::string h);
};

class Batter {
    private:
        std::string first_name;
        std::string last_name;
        std::string year;
        int player_id;
        std::string hand;

    public:
        Batter(std::string fn, std::string ln, std::string y);
        void set_id(int id);
        void set_hand(std::string h);
};

Pitcher read_pitcher_db(Pitcher pitcher);
Batter read_batter_db(Batter batter);

bool get_pitcher(std::string first_name, std::string last_name, std::string year);
bool get_batter(std::string first_name, std::string last_name, std::string year);

#endif