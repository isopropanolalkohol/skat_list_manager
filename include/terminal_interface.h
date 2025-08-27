//
// Created by joshua on 04.04.25.
//

#ifndef TERMINAL_INTERFACE_H
#define TERMINAL_INTERFACE_H
#include <iostream>
#include <functional>
#include "database_api.h"
enum COMMAND_RESULT
{
    SUCCESS, ERROR, EXIT
};

class command
{
    public:
    command(std::string cmd_name, std::function<COMMAND_RESULT (std::vector<std::string>, DatabaseAPI&)> cmd_function);
    std::string command_name;
    std::function<COMMAND_RESULT (std::vector<std::string>, DatabaseAPI& db)> command_function;
    COMMAND_RESULT execute(std::vector<std::string> args, DatabaseAPI& db) const;
};
std::vector<command> load_commands();

void MainLoop();

//main commands
COMMAND_RESULT cmd_change_player_list(std::vector<std::string> args, DatabaseAPI& db); //the players are stored in a global variable - change the database
COMMAND_RESULT cmd_get_player_list(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_make_entry(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_make_statistic(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_get_points(std::vector<std::string> args, const DatabaseAPI& db);
COMMAND_RESULT cmd_save(std::vector<std::string> args, const DatabaseAPI& db);
COMMAND_RESULT cmd_display_list(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_display_players(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_get_played_games(std::vector<std::string> args, const DatabaseAPI& db);

//utility commands
COMMAND_RESULT cmd_help(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_usage(std::vector<std::string> args, DatabaseAPI& db);
COMMAND_RESULT cmd_quit(std::vector<std::string> args, DatabaseAPI& db);



std::vector<std::string> tokenize(std::string input);


#endif //TERMINAL_INTERFACE_H
