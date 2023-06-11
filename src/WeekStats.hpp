#include "Globalstuff.hpp"
class WeekStats
{
public:
    WeekStats(int week_number_);
    void set_yellow_card();
    void set_red_card();
    void set_score( float score_);
    void set_injured();
    void set_goal_stats(int goals_, int assists_, int clean_sheets_);
    float get_score();
    int get_goals();
    int get_assists();
    int get_clean_sheets();
    bool get_has_yellow_card();
    bool get_has_red_card();
    bool get_is_injured();
private:
    float score;
    int goals;
    int assists;
    int clean_sheets;
    bool is_injured;
    bool has_yellow_card;
    bool has_red_card;
    int week_number;
};