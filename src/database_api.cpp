//
// Created by joshua on 04.04.25.
//
#include <iostream>
#include "database_api.h"
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include "terminal_interface.h"

DatabaseAPI::DatabaseAPI()
{  //login shit (I know this shit is hardcoded and insecure but idc since this isn't any valuable data to anyone else
    try
    {
        sql::Driver* driver = sql::mysql::get_driver_instance();
        connection = driver->connect("tcp://127.0.0.1:3306", "skat_api", "skat_api");
        connection->setSchema("skat_lists");
        std::cout << "slm >: Connected successfully!" << std::endl;
        current_list_id = -1;
    } catch (sql::SQLException& e)
    {
        std::cout << "slm >: error occurred. Database was not opened. Error: " << e.what() << std::endl;
        return;
    }
}
DatabaseAPI::~DatabaseAPI()
{
    delete connection;
    std::cout << "slm >: closed database connection" << std::endl;
    std::cout << "slm >: Bye!" << std::endl;

}
void DatabaseAPI::start_transaction() const
{
    try
    {
        sql::Statement* stmt = connection->createStatement();
        stmt->execute("START TRANSACTION");
        delete stmt;
    } catch (sql::SQLException& e)
    {
        std::cout << e.what() << std::endl;
    }

}
void DatabaseAPI::commit_transaction() const
{
    sql::Statement* stmt = connection->createStatement();
    stmt->execute("COMMIT");
    delete stmt;
}
void DatabaseAPI::rollback_transaction() const
{
    sql::Statement* stmt = connection->createStatement();
    stmt->execute("ROLLBACK");
    delete stmt;
}



void DatabaseAPI::create_users_if_dont_exist() const
{
    for (const auto& player : players)
    {
        sql::PreparedStatement* prepared_statement = connection->prepareStatement("SELECT * FROM players WHERE name = ?");
        prepared_statement->setString(1, to_upper(player));
        sql::ResultSet* result_set = prepared_statement->executeQuery();
        if (!result_set->next())
        {
            create_user(player);
        }
        delete result_set;
        delete prepared_statement;
    }
}


void DatabaseAPI::create_new_table() const
{
    create_users_if_dont_exist();
    //server abfragen ob Tabelle existiert
    if (get_table_id() == 0)
    {
        std::cout << "slm >: Creating new table..." << std::endl;
        try
        {
            sql::PreparedStatement* prep_statement = connection->prepareStatement("INSERT INTO lists(name) VALUES (?)");
            prep_statement->setString(1, getTableName(players));
            prep_statement->execute();
            sql::Statement* last_id_request = connection->createStatement();
            sql::ResultSet* res = last_id_request->executeQuery("SELECT LAST_INSERT_ID()");
            int last_insert_id;
            if (res->next())
            {
                last_insert_id = res->getInt(1);
            } else
            {
                delete prep_statement;
                delete last_id_request;
                delete res;
                return;
            }
            delete last_id_request;
            delete prep_statement;
            delete res;
            for (const auto& player : players)
            {
                int user_id = get_user_id(player);
                if (user_id == 0)
                {
                    std::cout << "Error occurred while creating table (ERROR10011)" << std::endl;
                    return;
                }
                /*sql::PreparedStatement* get_user_id_query = connection->prepareStatement("SELECT playerID FROM players WHERE name = ?");
                get_user_id_query->setString(1, player);
                sql::ResultSet* result = get_user_id_query->executeQuery();
                if (!result->next())
                {
                    user_id = result->getInt(1);
                    delete get_user_id_query;
                    delete result;
                }*/
                sql::PreparedStatement* insert_into_list = connection->prepareStatement("INSERT INTO players_lists(playerID, listID) VALUES (?, ?)");
                insert_into_list->setInt(1, user_id);
                insert_into_list->setInt(2, last_insert_id);
                insert_into_list->executeQuery();
                delete insert_into_list;
                //std::cout << "slm >: created new user. (" + player + ")" << std::endl;
            }
            std::cout << "slm >: new list successfully created." << std::endl;
        } catch (sql::SQLException& e)
        {
            std::cout << "slm >: error occurred while creating table: " << e.what() << std::endl;
        }
    }
}


void DatabaseAPI::delete_table() const
{

}

std::unordered_map<std::string, int> DatabaseAPI::get_points() const
{
    std::unordered_map<std::string, int> result;
    for (const auto& player : players)
    {
        sql::PreparedStatement* points_query = connection->prepareStatement("SELECT SUM((played_games.hasWon*3-2)*possibleGames.value) AS 'points' FROM played_games JOIN possibleGames ON played_games.playedGameID = possibleGames.gameID WHERE playerId = ? AND listID = ?");
        points_query->setInt(1, get_user_id(player));
        points_query->setInt(2, current_list_id);
        sql::ResultSet* result_set = points_query->executeQuery();
        if (result_set->next())
        {
            result[player] = result_set->getInt(1);
        }
        delete points_query;
        delete result_set;
    }
    return result;
}
//should work, returns 0 if doesn't exist
int DatabaseAPI::get_table_id() const
{
    try
    {
        std::string request_string = "SELECT listID FROM players_lists WHERE playerID IN " + generate_tuple_with_question_mark(players.size())
                                    + "GROUP BY listID HAVING COUNT(DISTINCT playerID) = ?"; //this query should work
        sql::PreparedStatement* all_players_in_list = connection->prepareStatement(request_string);
        int variable_count = 1;
        for (const auto& player : players)
        {
            //get each id
            sql::PreparedStatement* user_id_query = connection->prepareStatement("SELECT playerID FROM players WHERE name = ?");
            user_id_query->setString(1, to_upper(player));
            sql::ResultSet* res = user_id_query->executeQuery();
            if (!res->next())
            {
                //std::cout << "slm >: Error occurred while looking up userID." << std::endl;
                delete all_players_in_list;
                delete user_id_query;
                delete res;
                return 0;
            }
            int user_id = res->getInt(1);
            all_players_in_list->setInt(variable_count, user_id);
            variable_count++;
            delete user_id_query;
            delete res;
        }
        all_players_in_list->setInt(variable_count, players.size());
        sql::ResultSet* result_set = all_players_in_list->executeQuery();
        if (result_set->next())
        {
            int id = result_set->getInt(1);
            delete all_players_in_list;
            delete result_set;
            return id;
        }
        delete all_players_in_list;
        delete result_set;
        return 0;
    } catch (sql::SQLException& e)
    {
        std::cout << "slm >: error occurred: " << e.what() <<  std::endl;
        return true;
    }
}
int DatabaseAPI::get_user_id(std::string username) const
{
    sql::PreparedStatement* user_id_query = connection->prepareStatement("SELECT playerID FROM players WHERE name = ?");
    user_id_query->setString(1, to_upper(username));
    sql::ResultSet* res = user_id_query->executeQuery();
    int id;
    if (res->next())
    {
        id = res->getInt(1);
        delete user_id_query;
        delete res;
        return id;
    }
    delete user_id_query;
    delete res;
    return -1;
}
std::string DatabaseAPI::get_user_name(int id) const
{
    sql::PreparedStatement* get_name_query = connection->prepareStatement("SELECT name FROM players WHERE playerID = ?");
    get_name_query->setInt(1, id);
    sql::ResultSet* res = get_name_query->executeQuery();
    std::string name;
    if (res->next())
    {
        name = res->getString(1);
    }
    delete get_name_query;
    delete res;
    return name;
}


//checks if user already exists, and creates if not
void DatabaseAPI::create_user(std::string username) const
{
    std::cout << "slm >: creating new user..." << std::endl;
    try
    {
        sql::PreparedStatement* prepared_statement = connection->prepareStatement("SELECT * FROM players WHERE name = ?");
        prepared_statement->setString(1, to_upper(username));
        sql::ResultSet* result_set = prepared_statement->executeQuery();
        if (result_set->next())
        {
            std::cout << "slm >: user already exists!" << std::endl;
        } else
        {
            sql::PreparedStatement* create_user_statement = connection->prepareStatement("INSERT INTO players(name) VALUES (?)");
            create_user_statement->setString(1, to_upper(username));
            create_user_statement->execute();
            delete create_user_statement;
            std::cout << "slm >: user successfully created." << std::endl;
        }
        delete prepared_statement;
        delete result_set;
    } catch (sql::SQLException& e)
    {
        std::cout << "slm >: error occurred while trying to create user: " << e.what() << std::endl;
    }
}

void DatabaseAPI::login_user(const std::string& username)
{
    if (username.empty() || std::find(players.begin(), players.end(), username) != players.end())
    {
        std::cout << "slm >: user already exists or invalid name" << std::endl;
    }
    else
    {
        players.push_back(username);
    }
}

void DatabaseAPI::logout_user(std::string username)
{
    players.erase(std::remove(players.begin(), players.end(), username));
}
void DatabaseAPI::clear_users()
{
    players.clear();
}


void DatabaseAPI::show_logged_in_users() const
{
    std::cout << "slm >: logged in users are:";
    for (const auto& player : players)
    {
        std::cout << " " + player;
    }
    std::cout << std::endl;
}

void DatabaseAPI::set_list_id()
{
    if (int id = get_table_id(); id != 0)
    {
        current_list_id = id;
        return;
    }
    else
    {
        //create list;
    }
}


void DatabaseAPI::change_list()
{
    if (!players.empty())
    {
        std::cout << "slm >: the current list in use contains the players:";
        for (const auto& player : players)
        {
            std::cout << " " << to_upper(player);
        }
        std::cout << std::endl;
    }
    while (true)
    {
        std::cout << "slm >: Enter all the player names to log in, seperated by whitespace." << std::endl;
        std::string to_log_in_users;
        std::cout << ">: ";
        std::getline(std::cin, to_log_in_users);
        std::cout << "slm >: LOG IN current users: " << to_log_in_users << " ([y]/n)" << std::endl;
        std::string answer;
        std::cout << ">: ";
        std::getline(std::cin, answer);

        if (to_upper(answer) == "Y")
        {
            std::vector<std::string> usernames = tokenize(to_log_in_users);
            for (const auto username : usernames)
            {
                login_user(to_upper(username));
            }
            break;
        }
        else if (to_upper(answer) == "N")
        {
            std::cout << "slm >: Write the new users to log in." << std::endl;
            continue;
        }
        else
        {
            std::cout << "slm >: Invalid input! Answer with ([y]/n)" << std::endl;
        }
    }
}

void DatabaseAPI::push_entry(GameEntry entry) const
{
    try
    {
        //get id
        sql::PreparedStatement* get_id_query = connection->prepareStatement(
            "SELECT gameID FROM possibleGames WHERE value = ? AND typeName = ? AND peaks = ? AND modifierName = ?");
        get_id_query->setInt(1,calculate_game_value(entry));
        get_id_query->setString(2, return_type_name(entry.type));
        get_id_query->setInt(3, entry.peaks);
        get_id_query->setString(4, entry.modifier.name);
        sql::ResultSet* result_set = get_id_query->executeQuery();

        if (result_set->next())
        {
            int id = result_set->getInt(1);
            //std::cout << "Game found with id: " << id << std::endl;
            sql::PreparedStatement* insert_query =
                connection->prepareStatement("INSERT INTO played_games(playedGameID, playerID, listID, hasWon, time) VALUES (?,?,?,?, CURRENT_TIMESTAMP)");
            insert_query->setInt(1, id);
            insert_query->setInt(2, entry.user_id);
            insert_query->setInt(3, current_list_id);
            insert_query->setBoolean(4, entry.won);
            insert_query->executeQuery();
            delete insert_query;

        } else
        {
            std::cout << "slm >: error! game not found. (Database001)" << std::endl;
        }
        delete result_set;
        delete get_id_query;
    } catch (sql::SQLException& e)
    {
        std::cout << "slm >: error occurred: " << e.what() << std::endl;
    }
}

std::vector<std::string> DatabaseAPI::get_entries() const
{
    std::vector<std::string> entries;
    try
    {
        sql::PreparedStatement* get_games_query = connection->prepareStatement("SELECT players.name, possibleGames.typeName, possibleGames.peaks, possibleGames.modifierName, possibleGames.value, played_games.time, played_games.hasWon FROM played_games JOIN players ON players.playerID = played_games.playerID JOIN possibleGames ON played_games.playedGameID = possibleGames.gameID WHERE listID = ? ORDER BY played_games.gameID DESC LIMIT 10");
        get_games_query->setInt(1, current_list_id);
        sql::ResultSet* result_set = get_games_query->executeQuery();
        while (result_set->next())
        {
            std::string line = result_set->getString(1)
            + ": " + result_set->getString(2)
            + " m/o " + std::to_string(result_set->getInt(3))
            + ", " + result_set->getString(4)
            + ", value: " + std::to_string(((3*result_set->getBoolean(7)-2)*result_set->getInt(5)))
            + ", time: " + result_set->getString(6);
            entries.push_back(line);
        }
        delete result_set;
        delete get_games_query;
    } catch (sql::SQLException& e)
    {
        std::cout << "slm >: error occurred: " << e.what() << std::endl;
    }
    return entries;
}

int DatabaseAPI::get_list_id() const
{
    return current_list_id;
}
std::vector<std::string> DatabaseAPI::get_users() const
{
    return players;
}






bool isAlpha(std::string const &str) {
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
        std::string::npos;
}

bool validate_names(std::vector<std::string> players)
{
    for (const auto& name: players)
    {
        if (!isAlpha(name) || name.size() > 18 || name.size() == 0)
        {
            return false;
        }
    }
    return true;
}
std::string generate_tuple_with_question_mark(const int count)
{
    std::string str = "(";
    for (int i = 0; i < count; i++)
    {
        str += "?";
        if (i < count - 1)
        {
            str += ", ";
        }
    }
    str += ")";
    return str;
}


std::string to_upper(std::string str)
{
    std::string new_str = str;
    std::transform(new_str.begin(), new_str.end(),new_str.begin(), ::toupper);
    return new_str;
}

//only to be invoked after name validity was checked
std::string getTableName(std::vector<std::string> players)
{
    //make uppercase
    std::vector<std::string> sorted_player_names;
    std::string table_name = "";
    for (const auto& name: players)
    {
        sorted_player_names.push_back(to_upper(name));
    }

    std::sort(sorted_player_names.begin(), sorted_player_names.end());
    for (const auto& name: sorted_player_names)
    {
        table_name += name;
    }
    return table_name;
}


//TODO:
// Function that calculates game value
// Function that writes into the database this value
// Function that calculates total value for every player
// Function that makes certain statistics for example most played games etc.



// gameID (PK)  played_gameID (FK)     player (FK)      list (FK)         won/lost (bool)