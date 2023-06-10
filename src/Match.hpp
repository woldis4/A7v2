#ifndef _MATCH_
#define _MATCH_
#include "Globalstuff.hpp"
#include "Team.hpp"
class Match
{
public:
    Match(string home_ , string away_ , int home_goals_ , int away_goals_, 
        vector <string> team1_, vector <string> team2_, vector <pair<string, string> > goals_assists_);
    void update_players_score(); // ino bayad bezani
    string home;
    string away;
    int home_goals;
    int away_goals;
    vector <string> team1;
    vector <string> team2;
    vector <pair<string, string> > goals_assists;
};






#endif