//
// Created by joshua on 04.04.25.
//
#include "terminal_interface.h"
#include "database_api.h"
#include <utility>

#define VERSION "1.0.0 beta"

command::command(std::string cmd_name, std::function<COMMAND_RESULT (std::vector<std::string>, DatabaseAPI&)> cmd_function)
{
     command_name = std::move(cmd_name);
     command_function = std::move(cmd_function);
}
COMMAND_RESULT command::execute(std::vector<std::string> args, DatabaseAPI& db) const
{
     return command_function(std::move(args), db);
}

void CLI_MainLoop()

{
    //this acts like a terminal CLI interface. A window like system will follow soon.

    std::cout << "Welcome to the skat-list-manager, created and built by joshi.\n";
    std::cout << "version: " << VERSION << " copyright idk this shit 2025" << std::endl;

    std::cout << "slm >: establishing connection...\n";
    DatabaseAPI db;
    db.start_transaction();
    std::vector<command> commands = load_commands();
    db.change_list();
    db.create_new_table();
    db.set_list_id();
    db.show_logged_in_users();
    //main loop, task is to answer to commands on the command line.

    std::string input;
    bool quit = false;

    while (!quit)
    {
        std::cout << "user >: ";
        std::getline(std::cin, input);
        std::vector<std::string> command_line_args = tokenize(input);
        int command_index = -1;
        for (int i = 0; i < commands.size(); i++)
        {
            if (command_line_args[0] == commands[i].command_name)
            {
                command_index = i;
                break;
            }
        }
        if (command_index == -1)
        {
            std::cout << "slm >: command \"" << command_line_args[0] << "\" not found.\n";
            continue;
        }
        switch (commands[command_index].execute(command_line_args, db))
        {
            case EXIT:
                std::cout << "slm >: exiting... Bye!" << std::endl;
                quit = true;
                break;
            case ERROR:
                std::cout << "slm >: command failed. Please try again.\n";
                break;
            case SUCCESS:
                break;
            default:
                std::cout << "slm >: weird error occurred.\n";
        }
    }
    //make rollback/commit
    quit = false;
    while (!quit)
    {
        std::cout << "slm >: Do you want to save all the changes? ([y]/n) >: ";
        std::string ans;
        std::getline(std::cin, ans);
        ans = to_upper(ans);
        if (ans == "Y" || ans == "N")
        {
            switch (ans == "Y")
            {
                case true:
                    db.commit_transaction();
                    std::cout << "slm >: \033[32msaved changes.\033[0m" << std::endl;
                    break;
                case false:
                    db.rollback_transaction();
                    std::cout << "slm >: \033[31maborted transaction.\033[0m" << std::endl;
                    break;
                default:
                    std::cout << "slm >: Congratulations! You reached unreachable code..." << std::endl;
            }
            quit = true;
        }
        else
        {
            std::cout << "slm >: No valid command." << std::endl;
        }
    }
}


//command space
COMMAND_RESULT cmd_usage(std::vector<std::string> args, DatabaseAPI& db)
{
     if (args.size() != 2)
     {
          std::cout << "slm >: invalid argument count." << std::endl;
          return ERROR;
     }
     //TODO: display the usages
     return SUCCESS;
}
command sh_usage("usage", cmd_usage);

COMMAND_RESULT cmd_change_player_list(std::vector<std::string> args, DatabaseAPI& db)
{
     if (args.size() != 1)
     {
          std::cout << "slm >: invalid argument count." << std::endl;
          return ERROR;
     }
     std::cout << "slm >: all logged in players will be logged out." << std::endl;
     db.clear_users();
     db.change_list();
     db.create_new_table();
     db.set_list_id();
     return SUCCESS;
}
command sh_change_player_list("cpl", cmd_change_player_list);

COMMAND_RESULT cmd_make_entry(std::vector<std::string> args, DatabaseAPI& db)
{
     if (args.size() != 1)
     {
          std::cout << "slm >: invalid argument count." << std::endl;
          return ERROR;
     }
     GameEntry game = {UNSET, -1, Modifier{"UNSET", 0}, -1, false};
     while (true)
     {
          std::string choice;
          std::cout << "slm >: specify the game type. Options:\n1: Schellen:\n2: Rot\n3: Gruen\n4: Eichel:\n5: Grand:\n6: Null\n7: Null Hand\n8: Null ouvert\n9: Null ouvert Hand\n10: Ramsch\n0: abort\n";
          std::cout << "game type >: ";
          std::getline(std::cin, choice);
          if (choice == "0")
          {
               return ERROR;
          }
          if (choice == "1")
          {
               game.type = SCHELLEN;
               break;
          }
          else if (choice == "2")
          {
               game.type = ROT;
               break;
          }
          else if (choice == "3")
          {
               game.type = GRUEN;
               break;
          }
          else if (choice == "4")
          {
               game.type = EICHEL;
               break;
          }
          else if (choice == "5")
          {
               game.type = GRAND;
               break;
          }
          else if (choice == "6")
          {
               game.type = ZERO;
               game.peaks = 0;
               game.modifier = modifiers[0];
               break;
          }
          else if (choice == "7")
          {
               game.type = ZERO_HAND;
               game.peaks = 0;
               game.modifier = modifiers[6];
               break;
          }
          else if (choice == "8")
          {
               game.type = ZERO_OUVERT;
               game.peaks = 0;
               game.modifier = modifiers[0];
               break;
          }
          else if (choice == "9")
          {
               game.type = ZERO_HAND_OUVERT;
               game.peaks = 0;
               game.modifier = modifiers[6];
               break;
          }
          else if (choice == "10")
          {
               game.type = RAMSCH;
               game.peaks = 0;
               game.modifier = modifiers[0];
               game.won = false;
               break;
          }
     }
     if (game.peaks == -1)
     {
          std::cout << "peaks (use 0 for abort) >: ";
          //well i know this shit is boilerplate but idk
          while (true)
          {
               std::string peak_choice;
               std::getline(std::cin, peak_choice);
               if (peak_choice == "0")
               {
                    return ERROR;
               }
               else if (peak_choice == "1")
               {
                    game.peaks = 1;
                    break;
               }
               else if (peak_choice == "2")
               {
                    game.peaks = 2;
                    break;
               }
               else if (peak_choice == "3")
               {
                    game.peaks = 3;
                    break;
               }
               else if (peak_choice == "4")
               {
                    game.peaks = 4;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "5")
               {
                    game.peaks = 5;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "6")
               {
                    game.peaks = 6;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "7")
               {
                    game.peaks = 7;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "8")
               {
                    game.peaks = 8;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "9")
               {
                    game.peaks = 9;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "10")
               {
                    game.peaks = 10;
                    break;
               }
               else if (game.type != GRAND && peak_choice == "11")
               {
                    game.peaks = 11;
                    break;
               }
               else
               {
                    std::cout << "slm >: no valid peak number." << std::endl;
               }
          }
     }
     while (game.type == GRAND || game.type == SCHELLEN || game.type == ROT || game.type == GRUEN || game.type == EICHEL)
     {
          std::cout << "Input the modifier:\n1: Einfach\n2: Schneider\n3: Schwarz\n\n4: Hand Einfach\n5: Hand Schneider\n6: Hand Schwarz\n7: Hand Schneider angesagt:\n8: Hand Schneider angesagt, schwarz:\n9: Hand Schwarz angesagt\n10: Offen\n" << std::endl;
          std::cout << "modifier >: ";
          std::string choice;
          std::getline(std::cin, choice);
          if (choice == "0")
          {
               return ERROR;
          }
          else if (choice == "1")
          {
               game.modifier = modifiers[0];
               break;
          }
          else if (choice == "2")
          {
               game.modifier = modifiers[1];
               break;
          }
          else if (choice == "3")
          {
               game.modifier = modifiers[2];
               break;
          }
          else if (choice == "4")
          {
               game.modifier = modifiers[3];
               break;
          }
          else if (choice == "5")
          {
               game.modifier = modifiers[4];
               break;
          }
          else if (choice == "6")
          {
               game.modifier = modifiers[5];
               break;
          }
          else if (choice == "7")
          {
               game.modifier = modifiers[7];
               break;
          }
          else if (choice == "8")
          {
               game.modifier = modifiers[8];
               break;
          }
          else if (choice == "9")
          {
               game.modifier = modifiers[9];
               break;
          }
          else if (choice == "10")
          {
               game.modifier = modifiers[10];
               break;
          }
          else
          {
               std::cout << "slm >: no valid modifier." << std::endl;
          }
     }
     while (true)
     {
          std::cout << "slm >: what user played the game?\n0: abort\n" << std::endl;
          std::vector<std::string> players = db.get_users();
          for (int i = 0; i < players.size(); i++)
          {
               std::cout << i+1 << ": " << players[i] << std::endl;
          }
          std::cout << "player >: ";
          std::string choice;
          int int_choice;
          std::getline(std::cin, choice);
          try
          {
               int_choice = std::stoi(choice);
          } catch (const std::invalid_argument& e)
          {
               std::cout << "slm >: Invalid choice." << std::endl;
               continue;
          } catch (const std::out_of_range& e)
          {
               std::cout << "slm >: Invalid choice." << std::endl;
               continue;
          }
          if (int_choice < 0 || int_choice > players.size())
          {
               std::cout << "slm >: Invalid choice." << std::endl;
               continue;
          }
          if (int_choice == 0) return ERROR;
          game.user_id = db.get_user_id(players[int_choice-1]);
          std::cout << "user id: " << game.user_id << std::endl;
          break;
     }
     while (game.type != RAMSCH)
     {
          std::cout << "slm >: has the user won the game?\n0: lost\n1: won\n2: abort" << std::endl;
          std::string choice;
          std::cout << "won? >: ";
          std::getline(std::cin, choice);
          if (choice == "0")
          {
               game.won = false;
               break;
          }
          else if (choice == "1")
          {
               game.won = true;
               break;
          }
          else if (choice == "2")
          {
               return ERROR;
          }
          else
          {
               std::cout << "slm >: invalid choice." << std::endl;
          }
     }
     //DEBUG:
     /*
     std::string username;
     try
     {
          username = db.get_user_name(game.user_id);
     } catch (sql::SQLException& e)
     {
          std::cout << "slm >: invalid username." << std::endl;
          return ERROR;
     }
     std::cout << "DEBUG ENTRY: Type: " << game.type << ", peaks: " << game.peaks << ", modifier: " << game.modifier.name << ", player: ";
     std::cout << username << "(" << game.user_id << "), won: "<< game.won << std::endl;*/
     db.push_entry(game);
     std::cout << "slm >: entry was successfully added." << std::endl;
     return SUCCESS;
}
command sh_make_entry("me", cmd_make_entry);

COMMAND_RESULT cmd_get_points(std::vector<std::string> args, const DatabaseAPI &db)
{
     if (args.size() != 1)
     {
          std::cout << "slm >: invalid number of arguments." << std::endl;
          return ERROR;
     }
     std::unordered_map<std::string, int> points = db.get_points();
     for (const auto& point : points)
     {
          std::cout << point.first << ": " << point.second << std::endl;
     }
     return SUCCESS;
}
command sh_get_points("points", cmd_get_points);
COMMAND_RESULT cmd_save(std::vector<std::string> args, const DatabaseAPI &db)
{
     if (args.size() != 1)
     {
          return ERROR;
     }
     db.commit_transaction();
     db.start_transaction();
     std::cout << "slm >: saved!" << std::endl;
     return SUCCESS;
}
command sh_save("save", cmd_save);

COMMAND_RESULT cmd_get_played_games(std::vector<std::string> args, const DatabaseAPI &db)
{
     if (args.size() != 1)
     {
          return ERROR;
     }
     std::vector<std::string> entries = db.get_entries();
     for (const auto& entry : entries)
     {
          std::cout << entry << std::endl;
     }
     return SUCCESS;
}
command sh_get_played_games("gg", cmd_get_played_games);


COMMAND_RESULT cmd_quit(std::vector<std::string> args, DatabaseAPI& db)
{
     if (args.size() != 1)
     {
          std::cout << "slm >: Invalid number of arguments!" << std::endl;
          return ERROR;
     }
     return EXIT;
}
command sh_quit("quit", cmd_quit);

std::vector<command> load_commands()
{
     std::vector<command> commands;

     commands.clear();
     commands.push_back(sh_quit);
     commands.push_back(sh_change_player_list);
     commands.push_back(sh_make_entry);
     commands.push_back(sh_save);
     commands.push_back(sh_get_points);
     commands.push_back(sh_get_played_games);
     return commands;
}





std::vector<std::string> tokenize(const std::string input)
{
     std::vector<std::string> tokens;
     std::string token;
     for (char letter : input)
     {
          if (letter == ' ' && token.size() != 0)
          {
               tokens.push_back(token);
               token.clear();
          }
          else
          {
               token += letter;
          }
     }
     tokens.push_back(token);
     return tokens;
}

// TODO:
// Function for choosing the list to be in (like a menu, where I can go back or some)
//