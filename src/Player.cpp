#include "Player.hpp"

float the_absolute_insane_crazy_algorithm_for_finding_definitly_correct_score_for_players(float a)
{
    return 10 / (1 + exp(-a / 6));
}

Player::Player(string _name, int _price, int _role)
{
    can_play = true;
    price = _price;
    name = _name;
    role = _role;
    cnt_yellow_cards = 0;
    total_goals = 0;
    total_assists = 0;
    total_clean_sheets = 0;
    add_empty_week_stats(0);
}

int Player::get_role()
{
    return role;
}

string Player::get_name()
{
    return name;
}

void Player::add_empty_week_stats(int week_num)
{
    weekstats.push_back(WeekStats(week_num));
}

void Player::update_yellow_card(int week_num)
{
    cnt_yellow_cards++;
    weekstats[week_num].set_yellow_card();
}

void Player::update_red_card(int week_num)
{
    weekstats[week_num].set_red_card();
}

void Player::update_injured(int week_num)
{
    weekstats[week_num].set_injured();
}

void Player::update_score(int week_num, int pos, vector<int> team1_g, vector<int> team1_a,
                          vector<int> team1_og, vector<int> team2_g, vector<int> team2_a, vector<int> team2_og)
{
    int team1_goals = 0, team2_goals = 0;
    for (int i = 0; i < SQUAD_CNT; ++i)
        team1_goals += team1_g[i] + team2_og[i];
    for (int i = 0; i < SQUAD_CNT; ++i)
        team2_goals += team2_g[i] + team1_og[i];
    float week_score = 0;
    if (team1_goals > team2_goals)
        week_score += 5;
    else if (team1_goals == team2_goals)
        week_score += 1;
    else
        week_score -= 1;
    week_score -= 3 * team1_og[pos];
    week_score += find_individual_score(week_num, pos, team1_goals, team2_goals, team1_g,
                                        team1_a, team2_g);
    float the_most_correct_possible_value_for_a_player_score =
        the_absolute_insane_crazy_algorithm_for_finding_definitly_correct_score_for_players(week_score);
    weekstats[week_num].set_score(the_most_correct_possible_value_for_a_player_score);

    total_goals += team1_g[pos];
    total_assists += team1_a[pos];
    if (!team2_goals)
        total_clean_sheets += 1;
}

void Player::update_availability(int week_num)
{
    for (int i = week_num; i > max(0, week_num - RED_CARD_SUS_DAYS); --i)
    {
        if (weekstats[i].get_has_red_card())
        {
            cnt_yellow_cards = 0;
            can_play = false;
            return;
        }
    }
    for (int i = week_num; i > max(week_num - INJURY_SUS_DAYS, 0); --i)
    {
        if (weekstats[i].get_is_injured())
        {
            can_play = false;
            return;
        }
    }
    if (cnt_yellow_cards == 3)
    {
        cnt_yellow_cards = 0;
        can_play = false;
        return;
    }
    can_play = true;
}

float Player::find_player_score(int week_num)
{
    if (weekstats[week_num].get_score() == NOT_PLAYED_SCORE)
        return 0;
    return weekstats[week_num].get_score();
}

float Player::calculate_avarage_score()
{
    float sum = 0;
    int cnt = 0;
    for (WeekStats week : weekstats)
    {
        if (week.get_score() == NOT_PLAYED_SCORE)
            continue;
        cnt++;
        sum += week.get_score();
    }
    if (!cnt)
        return 0;
    return sum / cnt;
}

bool Player::is_available()
{
    return can_play;
}

int Player::get_price()
{
    return price;
}

Goalkeeper::Goalkeeper(string _name, int _price) : Player(_name, _price, GK)
{
}

string Goalkeeper::output_stats()
{
    ostringstream out;
    out << fixed << setprecision(1);
    out << name << " | role: " << ROLE_ABB_NAME[role] << " | score: " << calculate_avarage_score();
    out << " | cost: " << price << " | clean sheets: " << total_clean_sheets << endl;
    return out.str();
}

float Goalkeeper::find_individual_score(int week_num, int pos, int team1_score, int team2_score,
                                        vector<int> team1_g, vector<int> team1_a, vector<int> team2_g)
{
    float individual_score = 3;
    if (!team2_score)
        individual_score += 5;
    else
        individual_score += (-1) * team2_score;

    return individual_score;
}

Defender::Defender(string _name, int _price) : Player(_name, _price, DF)
{
}

string Defender::output_stats()
{
    ostringstream out;
    out << fixed << setprecision(1);
    out << name << " | role: " << ROLE_ABB_NAME[role] << " | score: " << calculate_avarage_score();
    out << " | cost: " << price << " | goals: " << total_goals << " | assists : ";
    out << total_assists << " | clean sheets: " << total_clean_sheets << endl;
    return out.str();
}

float Defender::find_individual_score(int week_num, int pos, int team1_score, int team2_score,
                                      vector<int> team1_g, vector<int> team1_a, vector<int> team2_g)
{
    float individual_score = 1;
    if (!team2_score)
        individual_score += 2;
    individual_score += team1_g[pos] * 4;
    individual_score += team1_a[pos] * 3;

    if (pos == LB)
        individual_score += (-1) * (team2_g[RB] + team2_g[RW]);

    if (pos == RB)
        individual_score += (-1) * (team2_g[LB] + team2_g[LW]);

    if (pos == CLB || pos == CRB)
        individual_score += (-1) * (team2_g[CLB] + team2_g[CRB] + team2_g[ST]);

    return individual_score;
}

Midfielder::Midfielder(string _name, int _price) : Player(_name, _price, MD)
{
}

string Midfielder::output_stats()
{
    ostringstream out;
    out << fixed << setprecision(1);
    out << name << " | role: " << ROLE_ABB_NAME[role] << " | score: " << calculate_avarage_score();
    out << " | cost: " << price << " | goals: " << total_goals << " | assists : ";
    out << total_assists << " | clean sheets: " << total_clean_sheets << endl;
    return out.str();
}

float Midfielder::find_individual_score(int week_num, int pos, int team1_score, int team2_score,
                                        vector<int> team1_g, vector<int> team1_a, vector<int> team2_g)
{
    float individual_score = 0;
    if (!team2_score)
        individual_score += 1;
    individual_score += team1_g[pos] * 3;
    individual_score += team1_a[pos] * 2;

    individual_score += (-1) * (team2_g[LM] + team2_g[CM] + team2_g[RM]);

    return individual_score;
}

Striker::Striker(string _name, int _price) : Player(_name, _price, FW)
{
}

string Striker::output_stats()
{
    ostringstream out;
    out << fixed << setprecision(1);
    out << name << " | role: " << ROLE_ABB_NAME[role] << " | score: " << calculate_avarage_score();
    out << " | cost: " << price << " | goals: " << total_goals << " | assists : ";
    out << total_assists << endl;
    return out.str();
}

float Striker::find_individual_score(int week_num, int pos, int team1_score, int team2_score,
                                     vector<int> team1_g, vector<int> team1_a, vector<int> team2_g)
{
    float individual_score = 0;
    if (!team1_g[pos])
        individual_score += -1;
    else
        individual_score += team1_g[pos] * 3;
    individual_score += team1_a[pos] * 1;

    return individual_score;
}