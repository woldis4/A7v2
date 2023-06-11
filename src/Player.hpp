#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_
#include "WeekStats.hpp"
#include "Globalstuff.hpp"


float the_absolute_insane_crazy_algorithm_for_finding_definitly_correct_score_for_players(float a);

class Player
{
public:
    Player() {};
    Player(string _name, int _price, int _role);
    string get_name();
    int get_role();
    void update_yellow_card(int week_num);
    void update_red_card(int week_num);
    void update_injured(int week_num);
    void update_score(int week_num, int pos, vector <int> team1_g, vector <int> team1_a, 
        vector <int> team1_og, vector <int> team2_g, vector <int> team2_a, vector <int> team2_og);
    void add_empty_week_stats(int week_num);
    void update_availability(int week_num);
    bool is_available();
    float find_player_score(int week_num);
    float calculate_avarage_score();
    int get_price();
    float virtual find_individual_score(int week_num, int pos, int team1_score, int team2_score, 
        vector <int> team1_g, vector <int> team1_a, vector <int> team2_g) = 0;
    string virtual output_stats() = 0;
protected:
    int price;
    string name;
    int total_goals;
    int total_assists;
    int total_clean_sheets;
    int role;
    int cnt_yellow_cards;
    bool can_play;
    vector<WeekStats> weekstats;
};

class Goalkeeper : public Player
{
public:
    Goalkeeper(string _name, int _price);
    float find_individual_score(int week_num, int pos, int team1_score, int team2_score, 
        vector <int> team1_g, vector <int> team1_a, vector <int> team2_g);
    string output_stats();
private:
};

class Defender : public Player
{
public:
    Defender(string _name, int _price);
    float find_individual_score(int week_num, int pos, int team1_score, int team2_score, 
        vector <int> team1_g, vector <int> team1_a, vector <int> team2_g);
    string output_stats();
private:
};

class Midfielder : public Player
{
public:
    Midfielder(string _name, int _price);
    float find_individual_score(int week_num, int pos, int team1_score, int team2_score, 
        vector <int> team1_g, vector <int> team1_a, vector <int> team2_g);
    string output_stats();
private:
};

class Striker : public Player
{
public:
    Striker(string _name, int _price);
    float find_individual_score(int week_num, int pos, int team1_score, int team2_score,
        vector <int> team1_g, vector <int> team1_a, vector <int> team2_g);
    string output_stats();
private:
};

#endif
