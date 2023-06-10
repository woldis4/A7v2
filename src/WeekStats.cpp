#ifndef _WEEKSTATS_HPP_
#define _WEEKSTATS_HPP_

#include "WeekStats.hpp"
WeekStats::WeekStats(int week_number_)
{
    week_number = week_number_;
    score = NOT_PLAYED_SCORE;
    has_yellow_card = false;
    has_red_card = false;
    is_injured = false;
}

void WeekStats::set_yellow_card()
{
    has_yellow_card = true;
}

void WeekStats::set_red_card()
{
    has_red_card = true;
}

void WeekStats::set_score( float score_)
{
    score = score_;
}

void WeekStats::set_injured()
{
    is_injured = true;
}

float WeekStats::get_score()
{
    return score;
}
bool WeekStats::get_has_yellow_card()
{
    return has_yellow_card;
}
bool WeekStats::get_has_red_card()
{
    return has_red_card;
}
bool WeekStats::get_is_injured()
{
    return is_injured;
}        

#endif