#include "Globalstuff.hpp"
class WeekStats
{
public:
    WeekStats(int week_number_);
    void set_yellow_card();
    void set_red_card();
    void set_score( float score_);
    void set_injured();
    float get_score();
    bool get_has_yellow_card();
    bool get_has_red_card();
    bool get_is_injured();
private:
    float score;
    bool is_injured;
    bool has_yellow_card;
    bool has_red_card;
    int week_number;
};