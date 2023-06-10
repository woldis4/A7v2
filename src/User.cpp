#include "User.hpp"

User::User()
{
    // do nothing
}

User::User(string username_, string password_, Team* team_)
{
    username = username_;
    password = password_;
    team = team_;
}