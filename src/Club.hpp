#ifndef _CLUB_HPP_
#define _CLUB_HPP_

#include "Globalstuff.hpp"
#include "Player.hpp"

class Club
{
public:
    Club(string _name);
    string get_name();
    int get_points();
    int get_goals_scored();
    int get_goals_conceded();
    void update_standing(int goals_scored_, int goals_conceded_, int point_);
    void update_player_scores(string home_team_name, string away_team_name, int home_team_score,
        int away_team_score, vector <string> team1, vector <string> team2,
        vector <pair<string, string> > goals_assists);
    void add_player(int pos, Player* p_player);
    vector <Player*> get_player(int role);
    vector<Player*> players[ROLE_CNT];
private:
    int points;
    int goals_scored;
    int goals_conceded;
    string name;
};

#endif