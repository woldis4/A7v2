#include "Globalstuff.hpp"

vector <string> split_line_into_words(string line, char delimiter)
{
    istringstream str_line(line);
    string word;
    vector <string> words;
    while (getline(str_line, word, delimiter)){
        if (word.back() == 13)
            word.pop_back();
        words.push_back(word);
    }
    return words;
}

int find_int_role(string str_role)
{
    for (int i = 0; i < ROLE_CNT; ++i)
        if (ROLE_NAME[i] == str_role || ROLE_ABB_NAME[i] == str_role)
            return i;
    throw NotFound();
}

string convert_underline_to_space(string team)
{
    for( int i = 0 ; i < team.size() ; ++i)
        if(team[i] == CLUB_DELIM)
            team[i] = SPACE;
    return team;
}

template<typename T>
vector<T> create_copy(vector<T> const &vec)
{
    vector<T> v(vec);
    return v;
}