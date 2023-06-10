#include "Match.hpp"
Match::Match(string home_, string away_, int home_goals_, int away_goals_,
             vector<string> team1_, vector<string> team2_, vector<pair<string, string>> goals_assists_)
{
    home = home_;
    away = away_;
    home_goals = home_goals_;
    away_goals = away_goals_;
    team1 = team1_;
    team2 = team2_;
    goals_assists = goals_assists_;
}

void Match::update_players_score(FutFan *futfan)
{
    for (string player : team1)
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            switch (futfan->find_player_by_name(player)->get_role())
            {
            case GK:
                week.score += 3;
                break;
            case DF:
                week.score += 1;
                break;
            default:
                break;
            }
    for (int i = 0; i < goals_assists.size(); ++i)
    {
        if (goals_assists[i].second == OWN_GOAL)
        {
            for (WeekStats week : futfan->find_player_by_name(goals_assists[i].first)->weekstats)
                week.score -= 3;
        }
        continue;
        for (WeekStats week : futfan->find_player_by_name(goals_assists[i].first)->weekstats)
            switch (
                futfan->find_player_by_name(goals_assists[i].first)->get_role())
            {
            case DF:
                week.score += 4;
                break;
            case MD:
                week.score += 3;
                break;
            case FW:
                week.score += 3;
                break;
            default:
                break;
            }

        for (WeekStats week : futfan->find_player_by_name(goals_assists[i].second)->weekstats)
            switch (
                futfan->find_player_by_name(goals_assists[i].first)->get_role())
            {
            case DF:
                week.score += 3;
                break;
            case MD:
                week.score += 2;
                break;
            case FW:
                week.score += 1;
                break;
            default:
                break;
            }
    }
    float point1 = 1, point2 = 1;
    if (home_goals > away_goals)
    {
        point1 = 5;
        point2 = -1;
    }
    else if (home_goals < away_goals)
    {
        point1 = -1;
        point2 = 5;
    }
    if (home_goals)
        for (WeekStats week : futfan->find_player_by_name(team2[GK])->weekstats)
            week.score -= home_goals;
    if (away_goals)
        for (WeekStats week : futfan->find_player_by_name(team1[GK])->weekstats)
            week.score -= away_goals;
    for (string player : team1)
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            if (!home_goals)
                switch (futfan->find_player_by_name(player)->get_role())
                {
                case GK:
                    week.score += 5;
                    break;
                case DF:
                    week.score += 2;
                    break;
                case MD:
                    week.score += 1;
                    break;
                default:
                    break;
                }
    for (string player : team2)
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            if (!away_goals)
                switch (futfan->find_player_by_name(player)->get_role())
                {
                case GK:
                    week.score += 5;
                    break;
                case DF:
                    week.score += 2;
                    break;
                case MD:
                    week.score += 1;
                    break;
                default:
                    break;
                }
    for (string player : team1)
    {
        bool forward_not_good = true;
        if (futfan->find_player_by_name(player)->get_role() != FW)
            forward_not_good = false;
        for (int i = 0; i < goals_assists.size(); ++i)
            if (goals_assists[i].first == player)
                forward_not_good = false;
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            if (forward_not_good)
                week.score -= 1;
    }
    for (string player : team2)
    {
        bool forward_not_good = true;
        if (futfan->find_player_by_name(player)->get_role() != FW)
            forward_not_good = false;
        for (int i = 0; i < goals_assists.size(); ++i)
            if (goals_assists[i].first == player)
                forward_not_good = false;
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            if (forward_not_good)
                week.score -= 1;
    }

    for (string player : team1)
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            week.score += point1;
    for (string player : team2)
        for (WeekStats week : futfan->find_player_by_name(player)->weekstats)
            week.score += point2;

    //role 3 and 4 not done
}