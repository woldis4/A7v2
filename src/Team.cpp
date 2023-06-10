#include "Team.hpp"

bool sort_by_player_name(Player* i, Player* j)
{
    return i->get_name() < j->get_name();
}

Team::Team(string team_name_)
{
    total_points = 0;
    team_name = team_name_;
    allowed_transfers = INF;
    official = false;
}

string Team::output_squad()
{
    ostringstream out;
    for (int i = 0; i < ROLE_CNT; ++i)
        if ((int)squad[i].size() != SQUAD_POS_NUM[i])
            throw Empty();
    
    for (int i = 0; i < ROLE_CNT; ++i)
        sort(squad[i].begin(), squad[i].end(), sort_by_player_name);
    
    out << "fantasy team: " << team_name << endl;

    int cnt = 0;
    for (int i = 0; i < ROLE_CNT; ++i)
        for (Player* player : squad[i])
            out << SQUAD_POS_STR[cnt] << ": " << player->get_name() << endl, cnt++;
    out << fixed << setprecision(SCORE_PRECISION);
    out << "Total Points: " << total_points;
    return out.str();
}

void Team::buy_player(Player* player)
{
    if (allowed_transfers <= 0)
        throw PermissionDenied();
    int player_role = player->get_role();
    if ((int)squad[player_role].size() == SQUAD_POS_NUM[player_role])
        throw BadRequest();
    squad[player_role].push_back(player);
    --allowed_transfers;

    if (official)
        return ;
    bool flag = true;
    for (int i = 0; i < ROLE_CNT; ++i)
        if ((int)squad[i].size() != SQUAD_POS_NUM[i])
            flag = false;
    if (flag)
    {
        official = true;
        allowed_transfers = 2;
    }
}

void Team::sell_player(string player_name)
{
    // there's an issue where if there's a player which is in the team 2 times, the code doesn't work
    if (allowed_transfers == 0)
        throw PermissionDenied();
    for (int i = 0; i < ROLE_CNT; ++i)
    {
        for (int j = 0; j < (int)squad[i].size(); ++j)
        {
            Player* player = squad[i][j];
            if (player->get_name() == player_name)
            {
                squad[i].erase(squad[i].begin() + j);
                --allowed_transfers;
                return ;
            }
        }
    }
    throw NotFound();
}

float Team::get_total_points()
{
    return total_points;
}

bool Team::is_official()
{
    return official;
}

string Team::get_team_name()
{
    return team_name;
}

void Team::update_new_week(int week_num)
{
    if (official)
        allowed_transfers = ALLOWED_TRANSFERS_WHEN_COMPLETE;
    else
        allowed_transfers = INF;

    float last_week_points = 0;
    bool squad_is_complete = true;
    for (int i = 0; i < ROLE_CNT; ++i)
    {
        if ((int)squad[i].size() != SQUAD_POS_NUM[i])
            squad_is_complete = false;
        for (Player* player : squad[i])
            last_week_points += player->calculate_avarage_score();
    }
    if (squad_is_complete)
        total_points += last_week_points;
}