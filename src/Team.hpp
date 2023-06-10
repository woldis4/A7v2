#ifndef _TEAM_HPP_
#define _TEAM_HPP_
#include "Globalstuff.hpp"
#include "Player.hpp"

bool sort_by_player_name(Player* i, Player* j);

class Team
{
public:
    Team(string team_name);
    bool is_official();
    float get_total_points();
    string output_squad();
    string get_team_name();
    void buy_player(Player* player);
    void sell_player(string player_name);
    void update_new_week(int week_num);
    void set_captain(Player* _captain);
    int get_budget();
private:
    int budget;
    float total_points;
    bool official;
    int allowed_transfers;
    string team_name;
    Player* captain;
    vector <Player*> squad[ROLE_CNT];
};

#endif