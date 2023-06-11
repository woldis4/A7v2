#include "FutFan.hpp"
bool cmp(Club *i, Club *j)
{
    if (i->get_points() < j->get_points())
        return false;
    if (i->get_points() == j->get_points() &&
        (i->get_goals_scored() - i->get_goals_conceded()) < (j->get_goals_scored() - j->get_goals_conceded()))
        return false;
    if (i->get_points() == j->get_points() &&
        (i->get_goals_scored() - i->get_goals_conceded()) == (j->get_goals_scored() - j->get_goals_conceded()) &&
        i->get_goals_scored() < i->get_goals_conceded())
        return false;
    return true;
}

bool cmp_player(Player *i, Player *j)
{
    if (i->calculate_avarage_score() < j->calculate_avarage_score())
        return false;
    if (i->calculate_avarage_score() == j->calculate_avarage_score() && i->get_name() < j->get_name())
        return false;
    return true;
}

FutFan::~FutFan()
{
    for (Player* player : players)
        delete player;
    for (Club* club : clubs)
        delete club;
    for (Team* team : teams)
        delete team;
    for (int i = 0; i < (int)all_matches.size(); ++i)
        for (Match* match : all_matches[i])
            delete match;
}

void FutFan::add_player(string player_name, int price, int role)
{
    for (Player *player : players)
        if (player->get_name() == player_name)
            return;
    if (role == GK)
        players.push_back(new Goalkeeper(player_name, price));
    if (role == DF)
        players.push_back(new Defender(player_name, price));
    if (role == MD)
        players.push_back(new Midfielder(player_name, price));
    if (role == FW)
        players.push_back(new Striker(player_name, price));
}

Team *FutFan::add_team(string team_name)
{
    Team *team = new Team(team_name);
    teams.push_back(team);
    return team;
}

Player *FutFan::find_player_by_name(string player_name)
{
    for (Player* player : players)
        if (player->get_name() == player_name)
            return player;
    throw NotFound();
}
Club *FutFan::find_club_by_name(string club_name)
{
    for (Club *club : clubs)
        if (club->get_name() == club_name)
            return club;
    throw NotFound();
}

Team *FutFan::find_team_by_name(string team_name)
{
    for (Team *team : teams)
        if (team->get_team_name() == team_name)
            return team;
    throw NotFound();
}

void FutFan::add_player_to_club(string club_name, string player_name)
{
    Club *p_club = find_club_by_name(club_name);
    Player *p_player = find_player_by_name(player_name);
    p_club->add_player(p_player->get_role(), p_player);
}

vector<vector<string>> FutFan::make_file_lines(string file_address, char delimiter)
{
    ifstream file;
    file.open(file_address);
    vector<vector<string>> lines;
    string line;
    while (getline(file, line))
    {
        vector<string> words = split_line_into_words(line, delimiter);
        lines.push_back(words);
    }
    file.close();
    return lines;
}

void FutFan::add_club(string club_name)
{
    clubs.push_back(new Club(club_name));
}

void FutFan::get_league_data(string file_address)
{
    vector<vector<string>> lines;
    lines = make_file_lines(file_address, COMMA);
    for (int line_num = 0; line_num < (int)lines.size(); ++line_num)
    {
        if (line_num == JUNK_LINE)
            continue;
        string club_name;
        for (int ind = 0; ind <= ROLE_CNT; ++ind)
        {
            if (ind == 0)
            {
                club_name = lines[line_num][ind];
                add_club(club_name);
                continue;
            }
            int pos_num = ind - 1;
            vector<string> pos_players = split_line_into_words(lines[line_num][ind], NAME_DELIM);
            for (string player_name_and_price : pos_players)
            {
                vector <string> split_name_and_price = split_line_into_words(player_name_and_price, PRICE_DELIM);
                string player_name = split_name_and_price[0];
                string price_str = split_name_and_price[1];
                price_str.pop_back();
                int price = stoi(price_str);
                add_player(player_name, price, pos_num);
                add_player_to_club(club_name, player_name);
            }
        }
    }
}

void FutFan::make_new_week_stats(int week_num)
{
    for (Player *player : players)
        player->add_empty_week_stats(week_num);
}

void FutFan::add_new_match(string home_team, string away_team, int home_team_score, int away_team_score,
                           vector <string> team1, vector <string> team2,
                           vector <pair<string, string> > goals_assists, int week_num)
{
    vector <Player*> team1_players, team2_players;
    for (string player_name : team1)
        team1_players.push_back(find_player_by_name(player_name));
    for (string player_name : team2)
        team2_players.push_back(find_player_by_name(player_name));
    Club *home_club = find_club_by_name(home_team);
    Club *away_club = find_club_by_name(away_team);

    Match* current_match = new Match(home_club, away_club, home_team_score, away_team_score,
                                     team1_players, team2_players, goals_assists);
    current_match->update_players_score(week_num);
    all_matches[week_num - 1].push_back(current_match);
    if (home_team_score == away_team_score)
    {
        home_club->update_standing(home_team_score, away_team_score, DRAW_POINT);
        away_club->update_standing(away_team_score, home_team_score, DRAW_POINT);
    }
    else if (home_team_score < away_team_score)
    {
        home_club->update_standing(home_team_score, away_team_score, LOSS_POINT);
        away_club->update_standing(away_team_score, home_team_score, WIN_POINT);
    }
    else
    {
        home_club->update_standing(home_team_score, away_team_score, WIN_POINT);
        away_club->update_standing(away_team_score, home_team_score, LOSS_POINT);
    }
}

void FutFan::update_match_stats(int week_num, vector<string> &data)
{
    string home_team, away_team;
    vector <pair<string, string> > goals_assists;
    vector <string> team1, team2;
    int home_team_score, away_team_score;
    for (int j = 0; j < (int)data.size(); ++j)
    {
        if (j == MATCH)
        {
            vector<string> match_teams = split_line_into_words(data[j], SCORE_DELIM);
            home_team = match_teams[0];
            away_team = match_teams[1];
        }
        if (j == RESULT)
        {
            vector<string> match_scores = split_line_into_words(data[j], SCORE_DELIM);
            home_team_score = stoi(match_scores[0]);
            away_team_score = stoi(match_scores[1]);
        }
        if (j == YELLOW_CARD)
        {
            vector<string> yellow_card_player = split_line_into_words(data[j], NAME_DELIM);
            for (string player_name : yellow_card_player)
                find_player_by_name(player_name)->update_yellow_card(week_num);
        }
        if (j == RED_CARD)
        {
            vector<string> red_card_player = split_line_into_words(data[j], NAME_DELIM);
            for (string player_name : red_card_player)
            {
                Player *player = find_player_by_name(player_name);
                find_player_by_name(player_name)->update_red_card(week_num);
            }
        }
        if (j == INJURED)
        {
            vector<string> injured_player = split_line_into_words(data[j], NAME_DELIM);
            for (string player_name : injured_player)
                find_player_by_name(player_name)->update_injured(week_num);
        }
        if (j == GOALS_AND_ASSISTS)
        {
            vector <string> goals_and_assists = split_line_into_words(data[j], NAME_DELIM);
            for (int i = 0; i < (int)goals_and_assists.size(); i++)
            {
                vector <string> goaler_plus_assister = split_line_into_words(goals_and_assists[i], SCORE_DELIM);
                goals_assists.push_back({goaler_plus_assister[0], goaler_plus_assister[1]});
            }
        }
        if (j == TEAM1)
            team1 = split_line_into_words(data[j], NAME_DELIM);
        if (j == TEAM2)
            team2 = split_line_into_words(data[j], NAME_DELIM);
    }
    add_new_match(home_team, away_team, home_team_score, away_team_score, team1, team2, goals_assists, week_num);
}

void FutFan::update_week_stats(int week_num)
{
    vector <Match*> empty_vec;
    all_matches.push_back(empty_vec); // its for initializing the week so we can push matches
    for (Player *player : players)
    {
        player->add_empty_week_stats(week_num);
    }
    string file_name = WEEK_STATS_FOLDER + "week_" + to_string(week_num) + ".csv";
    vector<vector<string> > week = make_file_lines(file_name, COMMA);
    for (int i = 0; i < (int)week.size(); ++i)
    {
        if (i == JUNK_LINE)
            continue;
        update_match_stats(week_num, week[i]);
    }
    for (Team *team : teams)
        team->update_new_week(week_num);
    for (Player *player : players)
    {
        player->update_availability(week_num);
    }
}

string FutFan::output_standing()
{
    ostringstream out;
    sort(clubs.begin(), clubs.end(), cmp);
    out << "league standing:" << endl;
    for (int i = 0; i < (int)clubs.size(); ++i)
    {
        Club *club = clubs[i];
        out << i + 1 << ". " << club->get_name() << ": ";
        out << "score: " << club->get_points() << " | ";
        out << "GF: " << club->get_goals_scored() << " | ";
        out << "GA: " << club->get_goals_conceded() << endl;
    }
    return out.str();
}

string FutFan::team_of_the_week(int week_num)
{
    ostringstream out;
    vector<string> team_of_week = {"", "", "", "", ""};
    float goalkeeper_max = 0, defender_max1 = 0, defender_max2 = 0;
    float midfielder_max = 0, forward_max = 0;
    for (Player *player : players)
    {
        if (player->find_player_score(week_num) >= goalkeeper_max && player->get_role() == GK)
        {
            goalkeeper_max = player->find_player_score(week_num);
            team_of_week[0] = player->get_name();
        }
        if (player->find_player_score(week_num) >= defender_max1 && player->get_role() == DF)
        {
            defender_max1 = player->find_player_score(week_num);
            team_of_week[1] = player->get_name();
        }
        else if (player->find_player_score(week_num) >= defender_max2 && player->get_role() == DF)
        {
            defender_max2 = player->find_player_score(week_num);
            team_of_week[2] = player->get_name();
        }
        if (player->find_player_score(week_num) >= midfielder_max && player->get_role() == MD)
        {
            midfielder_max = player->find_player_score(week_num);
            team_of_week[3] = player->get_name();
        }
        if (player->find_player_score(week_num) >= forward_max && player->get_role() == FW)
        {
            forward_max = player->find_player_score(week_num);
            team_of_week[4] = player->get_name();
        }
    }
    out << fixed << setprecision(SCORE_PRECISION);
    out << "team of the week:" << endl
        << ROLE_NAME[0] << ": " << team_of_week[0] << " | score: " << goalkeeper_max << endl
        << ROLE_NAME[1] << "1: " << team_of_week[1] << " | score: " << defender_max1 << endl
        << ROLE_NAME[1] << "2: " << team_of_week[2] << " | score: " << defender_max2 << endl
        << ROLE_NAME[2] << ": " << team_of_week[3] << " | score: " << midfielder_max << endl
        << ROLE_NAME[3] << ": " << team_of_week[4] << " | score: " << forward_max << endl;
    return out.str();
}

string FutFan::print_players(vector<Player *> club_players)
{
    ostringstream out;
    out << fixed << setprecision(SCORE_PRECISION);
    out << "list of players:" << endl;
    for (int i = 0; i < club_players.size(); ++i)
        out << i << ". " << club_players[i]->output_stats();
    return out.str();
}

string FutFan::output_players(bool ranked, int position, Club *club)
{
    if (position == -1 && !ranked)
    {
        vector<Player *> club_players;
        for (int i = 0; i < ROLE_CNT; i++)
            for (Player *player : club->players[i])
                if (player->get_name() == player->get_name())
                    club_players.push_back(player);
        return print_players(club_players);
    }
    if (ranked && position == -1)
    {
        sort(players.begin(), players.end(), cmp_player);
        return print_players(players);
    }
    if (ranked)
        sort(club->players[position].begin(), club->players[position].end(), cmp_player);
    return print_players(club->players[position]);
    if (position != -1)
    {
        return print_players(club->players[position]);
    }
}

string FutFan::matchs_of_the_week(int week_num)
{
    ostringstream out;
    for (Match* match : all_matches[week_num - 1])
    {
        out << match->output_match();
    }
    return out.str();
}

bool FutFan::check_team(string team_name)
{
    for (Team *t : teams)
        if (t->get_team_name() == team_name)
            return false;
    return true;
}

void FutFan::pass_week(int week_num)
{
    update_week_stats(week_num);
}