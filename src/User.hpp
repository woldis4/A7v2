#ifndef _USER_HPP_
#define _USER_HPP_

#include "Globalstuff.hpp"
#include "Team.hpp"

class User
{
public:
    User();
    User(string username, string password, Team* team);
    string username;
    string password;
    Team* team;
private:
};

#endif