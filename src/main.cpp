#include "Globalstuff.hpp"
#include "FutFan.hpp"
#include "CommandHandler.hpp"
int main()
{
    FutFan futfan;
    futfan.get_league_data(LEAGUE_ADDRESS);
    CommandHandler command_handler(&futfan);
    command_handler.handle_commands();
    return 0;
}