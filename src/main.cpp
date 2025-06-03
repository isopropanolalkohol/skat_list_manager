#include <iostream>
#include <vector>
#include <cppconn/exception.h>

#include "database_api.h"
#include "terminal_interface.h"
#define VERSION "1.0.0 beta"

int main()
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

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.