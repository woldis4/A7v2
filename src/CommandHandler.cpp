#include "CommandHandler.hpp"

CommandHandler::CommandHandler(FutFan *futfan_)
{
    futfan = futfan_;
    session = new Session();
}

int CommandHandler::find_command_num(vector<string> command_words)
{
    if ((int)command_words.size() < 2)
        throw BadRequest();
    int cnt = 0;
    for (CommandType cmd : COMMAND_TYPE)
    {
        if (cmd.command_class == command_words[0] && cmd.command_type == command_words[1])
            return cnt;
        cnt++;
    }
    throw BadRequest();
}

string CommandHandler::signup_user(vector<string> command)
{
    if ((int)command.size() != 7 || command[2] != "?" || command[3] != "team_name" || command[5] != "password")
        return BAD_REQUEST;
    string team_name = command[4];
    string password = command[6];
    if (session->is_admin_logged_in || session->is_user_logged_in)
        return PERMISSION_DENIED;
    if (!session->is_username_available(team_name))
        return BAD_REQUEST;
    session->add_user(team_name, password, futfan->add_team(team_name));
    return OK;
}

string CommandHandler::login_user(vector<string> command)
{
    if ((int)command.size() != 7 || command[2] != "?" || command[3] != "team_name" || command[5] != "password")
        return BAD_REQUEST;
    string team_name = command[4];
    string password = command[6];
    if (session->is_admin_logged_in || session->is_user_logged_in)
        return PERMISSION_DENIED;
    try
    {
        session->login_user(team_name, password);
    }
    catch (NotFound &err)
    {
        return err.out();
    }
    catch (PermissionDenied &err)
    {
        return err.out();
    }
    return OK;
}

string CommandHandler::logout_user(vector<string> command)
{
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    if (!session->is_admin_logged_in && !session->is_user_logged_in)
        return PERMISSION_DENIED;
    session->logout_user();
    return OK;
}

string CommandHandler::register_admin(vector<string> command)
{
    if ((int)command.size() != 7 || command[2] != "?" || command[3] != "username" || command[5] != "password")
        return BAD_REQUEST;
    string username = command[4];
    string password = command[6];
    if (session->is_admin_logged_in || session->is_user_logged_in)
        return PERMISSION_DENIED;
    try
    {
        session->register_admin(username, password);
    }
    catch (BadRequest &err)
    {
        return err.out();
    }
    return OK;
}

string CommandHandler::close_transfer_window(vector<string> command)
{
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    try
    {
        session->close_transfer_window();
    }
    catch (PermissionDenied &err)
    {
        return err.out();
    }
    return OK;
}

string CommandHandler::open_transfer_window(vector<string> command)
{
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    try
    {
        session->open_transfer_window();
    }
    catch (PermissionDenied &err)
    {
        return err.out();
    }
    return OK;
}

string CommandHandler::get_league_standing(vector<string> command)
{
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    return futfan->output_standing();
}

string CommandHandler::get_team_of_the_week(vector<string> command)
{
    bool with_week_num = false;
    if ((int)command.size() == 3)
        if (command[2] != "?")
            return BAD_REQUEST;
        else if ((int)command.size() == 5)
        {
            with_week_num = true;
            if (command[2] != "?" || command[3] != "week_num")
                return BAD_REQUEST;
        }
        else
            return BAD_REQUEST;
    if (with_week_num){
        if (stoi(command[4]) > session->current_week_num)
            return BAD_REQUEST;
        return futfan->team_of_the_week(stoi(command[4]));
    }
    return futfan->team_of_the_week(session->current_week_num);
}

string CommandHandler::get_players(vector<string> command)
{
    try
    {
        string role = "";
        string team_name = convert_underline_to_space(command[4]);
        bool ranked = false, with_position = false;
        int role_num = -1;
        if (command.back() == "ranks")
            ranked = true;
        if (command[2] != "?" || command[3] != "team_name")
            return BAD_REQUEST;
        if ((int)command.size() == 7 && !ranked)
            return BAD_REQUEST;
        if ((int)command.size() == 7 || ((int)command.size() == 6 && !ranked))
            for (int i = 0; i < ROLE_CNT; ++i)
                if (ROLE_ABB_NAME[i] == command[5])
                {
                    role_num = i;
                    role = command[5];
                }
        if ((int)command.size() == 5 || ranked)
            role = "no roles";
        if (role == "")
            return BAD_REQUEST;
        else
            with_position = true;
        Club *club = futfan->find_club_by_name(team_name);
        return futfan->output_players(ranked, role_num, club);
    } catch (NotFound &not_found){
        return NOT_FOUND;
    } catch (BadRequest &bad){
        return BAD_REQUEST;
    }
}

string CommandHandler::get_match_results(vector<string> command)
{
    bool with_week_num = false;
    if ((int)command.size() == 3)
        if(command[2] != "?")
            return BAD_REQUEST;
    if ((int)command.size() == 5){
        with_week_num = true;
        if(command[2] != "?" || command[3] != "week_num")
            return BAD_REQUEST;
        if(stoi(command[4]) > session->current_week_num)
            return BAD_REQUEST;
    }
    if(with_week_num)
        return futfan->matchs_of_the_week(stoi(command[4]));
    return futfan->matchs_of_the_week(session->current_week_num);
}

string CommandHandler::buy_player(vector <string> command)
{
    if ((int)command.size() < 5 || command[2] != "?" || command[3] != "name")
        return BAD_REQUEST;
    if (!session->is_user_logged_in)
        return PERMISSION_DENIED;
    if (!session->is_transfer_window_open)
        return PERMISSION_DENIED;
    string player_name = command[4];
    for (int i = 5; i < (int)command.size(); ++i)
        player_name += " ", player_name += command[i];
    Player* chosen_player;
    try
    {
        chosen_player = futfan->find_player_by_name(player_name);
    } catch (NotFound &err){
        return err.out();
    }
    if (!chosen_player->is_available())
        return NOT_AVAILABLE_FOR_PURCHASE;
    string current_user_teamname = session->get_current_user_teamname();
    Team* current_user_team = futfan->find_team_by_name(current_user_teamname);
    try
    {
        current_user_team->buy_player(chosen_player);
    } catch (BadRequest &err){
        return err.out();
    } catch (PermissionDenied &err){
        return err.out();
    }
    return OK;
}

string CommandHandler::get_squad(vector <string> command)
{
    if (!session->is_user_logged_in)
        return PERMISSION_DENIED;
    if ((int)command.size() != 3 && (int)command.size() != 5)
        return BAD_REQUEST;
    if (command[2] != "?")
        return BAD_REQUEST;
    if ((int)command.size() == 5 && command[3] != "fantasy_team")
        return BAD_REQUEST;
    Team* chosen_team;
    try
    {
        string team_name = session->get_current_user_teamname();
        chosen_team = futfan->find_team_by_name(team_name);
        if ((int)command.size() == 5)
            chosen_team = futfan->find_team_by_name(command[4]);
    } catch (NotFound &err){
        return err.out();
    }
    try
    {
        return chosen_team->output_squad();
    } catch (Empty &err){
        return err.out();
    }
}

string CommandHandler::sell_player(vector <string> command)
{
    if ((int)command.size() < 5 || command[2] != "?" || command[3] != "name")
        return BAD_REQUEST;
    if (!session->is_user_logged_in)
        return PERMISSION_DENIED;
    if (!session->is_transfer_window_open)
        return PERMISSION_DENIED;
    string player_name = command[4];
    for (int i = 5; i < (int)command.size(); ++i)
        player_name += " ", player_name += command[i];
    string current_user_teamname = session->get_current_user_teamname();
    Team* current_user_team = futfan->find_team_by_name(current_user_teamname);
    try
    {
        current_user_team->sell_player(player_name);
    } catch(NotFound &err){
        return err.out();
    } catch (PermissionDenied &err){
        return err.out();
    }
    return OK;
}

string CommandHandler::set_captain(vector <string> command)
{
    if ((int)command.size() < 5 || command[2] != "?" || command[3] != "name")
        return BAD_REQUEST;
    if (!session->is_user_logged_in)
        return PERMISSION_DENIED;
    string team_name = session->get_current_user_teamname();
    string captain_name = command[4];
    for (int i = 5; i < (int)command.size(); ++i)
    {
        captain_name += " ";
        captain_name += command[i];
    }
    Team* my_team = futfan->find_team_by_name(team_name);
    Player* captain = futfan->find_player_by_name(captain_name);
    try
    {
        my_team->set_captain(captain);
    } catch(NotFound& err){
        return err.out();
    }
    return OK;
}

string CommandHandler::show_budget(vector <string> command)
{
    ostringstream out;
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    if (!session->is_user_logged_in)
        return PERMISSION_DENIED;
    string team_name = session->get_current_user_teamname();
    Team* my_team = futfan->find_team_by_name(team_name);
    out << my_team->get_budget();
    return out.str();
}

string CommandHandler::get_users_ranking(vector <string> command)
{
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    return session->get_users_ranking();
}

string CommandHandler::pass_week(vector <string> command)
{
    if ((int)command.size() != 2)
        return BAD_REQUEST;
    if (!session->is_admin_logged_in)
        return PERMISSION_DENIED;
    session->current_week_num++;
    futfan->pass_week(session->current_week_num);
    return OK;
}

void CommandHandler::handle_commands()
{
    string line;
    while (getline(cin, line))
    {
        vector<string> command_words = split_line_into_words(line, SPACE);
        int command_num;
        try
        {
            command_num = find_command_num(command_words);
        }
        catch (BadRequest &err)
        {
            cout << err.out() << endl;
            continue;
        }
        if (command_num == SIGNUP)
            cout << signup_user(command_words) << endl;

        if (command_num == LOGIN)
            cout << login_user(command_words) << endl;

        if (command_num == LOGOUT)
            cout << logout_user(command_words) << endl;

        if (command_num == REGISTER_ADMIN)
            cout << register_admin(command_words) << endl;

        if (command_num == OPEN_TRANSFER_WINDOW)
            cout << open_transfer_window(command_words) << endl;

        if (command_num == CLOSE_TRANSFER_WINDOW)
            cout << close_transfer_window(command_words) << endl;

        if (command_num == LEAGUE_STANDINGS)
            cout << get_league_standing(command_words);

        if (command_num == TEAM_OF_THE_WEEK)
            cout << get_team_of_the_week(command_words) << endl;

        if (command_num == PLAYERS)
            cout << get_players(command_words)<<endl;

        if (command_num == MATCHES_RESULT_LEAGUE)
            cout << get_match_results(command_words);

        if (command_num == SQUAD)
            cout << get_squad(command_words) << endl;

        if (command_num == BUY_PLAYER)
            cout << buy_player(command_words) << endl;

        if (command_num == SELL_PLAYER)
            cout << sell_player(command_words) << endl;

        if (command_num == SET_CAPTAIN)
            cout << set_captain(command_words) << endl;

        if (command_num == SHOW_BUDGET)
            cout << show_budget(command_words) << endl;

        if (command_num == USERS_RANKING)
            cout << get_users_ranking(command_words);

        if (command_num == PASS_WEEK)
            cout << pass_week(command_words) << endl;
    }
}
