#ifndef _MATCH_
#define _MATCH_
#include "Globalstuff.hpp"
#include "Club.hpp"
class Match
{
public:
    Match(Club* home_ , Club* away_ , int home_goals_ , int away_goals_, 
        vector <Player*> team1_, vector <Player*> team2_, vector <pair<string, string> > goals_assists_);
    void update_players_score(int week_num);
    string output_match();
private:
    Club* home;
    Club* away;
    int home_goals;
    int away_goals;
    vector <Player*> team1;
    vector <Player*> team2;
    vector <pair<string, string> > goals_assists;
};






#endif