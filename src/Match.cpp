#include "Match.hpp"
Match::Match(Club* home_ , Club* away_ , int home_goals_ , int away_goals_, 
        vector <Player*> team1_, vector <Player*> team2_, vector <pair<string, string> > goals_assists_)
{
    home = home_;
    away = away_;
    home_goals = home_goals_;
    away_goals = away_goals_;
    team1 = team1_;
    team2 = team2_;
    goals_assists = goals_assists_;
}

string Match::output_match()
{
    ostringstream out;
    out <<  home->get_name() << SPACE << home_goals
        << " | " << away->get_name() << SPACE << away_goals << endl;
    return out.str();
}

void Match::update_players_score(int week_num)
{
    vector <int> team1_goals(SQUAD_CNT, 0);
    vector <int> team2_goals(SQUAD_CNT, 0);
    vector <int> team1_assists(SQUAD_CNT, 0);
    vector <int> team2_assists(SQUAD_CNT, 0);
    vector <int> team1_owngoals(SQUAD_CNT, 0);
    vector <int> team2_owngoals(SQUAD_CNT, 0);
    for (pair <string, string> goal_assist : goals_assists)
    {
        if (goal_assist.second == OWN_GOAL)
        {
            for (int i = 0; i < SQUAD_CNT; ++i)
            {
                if (goal_assist.first == team1[i]->get_name())
                    team1_owngoals[i]++;
                if (goal_assist.first == team2[i]->get_name())
                    team2_owngoals[i]++;
            }
            continue;
        }

        for (int i = 0; i < SQUAD_CNT; ++i)
        {
            if (goal_assist.first == team1[i]->get_name())
                team1_goals[i]++;
            if (goal_assist.first == team2[i]->get_name())
                team2_goals[i]++;

            if (goal_assist.second == team1[i]->get_name())
                team1_assists[i]++;
            if (goal_assist.second == team2[i]->get_name())
                team2_assists[i]++;
        }
    }

    for (int i = 0; i < SQUAD_CNT; ++i)
    {
        team1[i]->update_score(week_num, i, team1_goals, team1_assists, team1_owngoals,
            team2_goals, team2_assists, team2_owngoals);
        team2[i]->update_score(week_num, i, team2_goals, team2_assists, team2_owngoals,
            team1_goals, team1_assists, team1_owngoals);
    }
}