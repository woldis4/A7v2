#include "Club.hpp"

int Club::get_points()
{
    return points;
}

int Club::get_goals_scored()
{
    return goals_scored;
}

int Club::get_goals_conceded()
{
    return goals_conceded;
}

Club::Club(string _name)
{
    name = _name;
    goals_conceded = 0;
    goals_scored = 0;
    points = 0;
}

string Club::get_name()
{
    return name;
}

void Club::update_standing(int goals_scored_, int goals_conceded_, int point_)
{
    goals_conceded += goals_conceded_;
    goals_scored += goals_scored_;
    points += point_;
}

void Club::add_player(int role, Player* p_player){
    players[role].push_back(p_player);
}

vector <Player*> Club::get_player(int role)
{
    vector <Player*> cur_players;
    for(Player* player : players[role])
        cur_players.push_back(player);
    return cur_players;
}