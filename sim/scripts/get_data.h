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
        const std::string& get_first_name() const;
        const std::string& get_last_name() const;
        const std::string& get_year() const;
        const int& get_id() const;
        const std::string& get_hand() const;
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
        const std::string& get_first_name() const;
        const std::string& get_last_name() const;
        const std::string& get_year() const;
        const int& get_id() const;
        const std::string& get_hand() const;
};

bool read_pitcher_db(Pitcher pitcher);
bool read_batter_db(Batter batter);

bool get_pitcher(std::string first_name, std::string last_name, std::string year);
bool get_batter(std::string first_name, std::string last_name, std::string year);

#endif