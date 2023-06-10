#include "Match.hpp"
Match::Match(string home_ , string away_ , int home_goals_ , int away_goals_, 
        vector <string> team1_, vector <string> team2_, vector <pair<string, string> > goals_assists_)
{
    home = home_;
    away = away_;
    home_goals = home_goals_;
    away_goals = away_goals_;
    team1 = team1_;
    team2 = team2_;
    goals_assists = goals_assists_;
}