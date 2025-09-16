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
        connection_ = driver->connect("tcp://127.0.0.1:3306", "skat_api", "skat_api");
        connection_->setSchema("skat_lists");
        //std::cout << "slm >: Connected successfully!" << std::endl;
        currentListId_ = -1;
    } catch (sql::SQLException& e)
    {
        //std::cout << "slm >: error occurred. Database was not opened. Error: " << e.what() << std::endl;
        return;
    }
}
DatabaseAPI::~DatabaseAPI()
{
    delete connection_;
    //std::cout << "slm >: closed database connection_" << std::endl;
    //std::cout << "slm >: Bye!" << std::endl;

}
void DatabaseAPI::startTransaction() const
{
    try
    {
        sql::Statement* stmt = connection_->createStatement();
        stmt->execute("START TRANSACTION");
        delete stmt;
    } catch (sql::SQLException& e)
    {
        std::cout << e.what() << std::endl;
    }

}
void DatabaseAPI::commitTransaction() const
{
    sql::Statement* stmt = connection_->createStatement();
    stmt->execute("COMMIT");
    delete stmt;
}
void DatabaseAPI::rollbackTransaction() const
{
    sql::Statement* stmt = connection_->createStatement();
    stmt->execute("ROLLBACK");
    delete stmt;
}



void DatabaseAPI::createUsersIfDontExist() const
{
    for (const auto& player : players_)
    {
        std::string query = "SELECT * FROM players WHERE name = ?";
        sql::PreparedStatement* prepared_statement = connection_->prepareStatement(query);
        prepared_statement->setString(1, to_upper(player));
        sql::ResultSet* result_set = prepared_statement->executeQuery();
        if (!result_set->next())
        {
            createUser(player);
        }
        delete result_set;
        delete prepared_statement;
    }
}


void DatabaseAPI::createNewTable() const
{
    createUsersIfDontExist();
    //server abfragen ob Tabelle existiert
    if (getTableId() == 0)
    {
        std::cout << "slm >: Creating new table..." << std::endl;
        try
        {
            sql::PreparedStatement* prep_statement = connection_->prepareStatement("INSERT INTO lists(name) VALUES (?)");
            prep_statement->setString(1, getTableName(players_));
            prep_statement->execute();
            sql::Statement* last_id_request = connection_->createStatement();
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
            for (const auto& player : players_)
            {
                int user_id = getUserId(player);
                if (user_id == 0)
                {
                    std::cout << "Error occurred while creating table (ERROR10011)" << std::endl;
                    return;
                }
                /*sql::PreparedStatement* get_user_id_query = connection_->prepareStatement("SELECT playerID FROM players WHERE name = ?");
                get_user_id_query->setString(1, player);
                sql::ResultSet* result = get_user_id_query->executeQuery();
                if (!result->next())
                {
                    user_id = result->getInt(1);
                    delete get_user_id_query;
                    delete result;
                }*/
                sql::PreparedStatement* insert_into_list = connection_->prepareStatement("INSERT INTO players_lists(playerID, listID) VALUES (?, ?)");
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


void DatabaseAPI::deleteTable() const
{

}

std::unordered_map<std::string, int> DatabaseAPI::getPoints() const
{
    std::unordered_map<std::string, int> result;
    for (const auto& player : players_)
    {
        std::string query = std::string("SELECT SUM((played_games.hasWon*3-2)*possibleGames.score) AS 'points' FROM played_games ")
        + std::string("JOIN possibleGames ON played_games.playedGameID = possibleGames.gameID WHERE playerId = ? AND listID = ?");
        sql::PreparedStatement* points_query = connection_->prepareStatement(query);
        points_query->setInt(1, getUserId(player));
        points_query->setInt(2, currentListId_);
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
int DatabaseAPI::getTableId() const
{
    try
    {
        std::string request_string = "SELECT listID FROM players_lists WHERE playerID IN "
        + generate_tuple_with_question_mark(static_cast<int>(players_.size()))
        + "GROUP BY listID HAVING COUNT(DISTINCT playerID) = ?"; //this query should work
        sql::PreparedStatement* all_players_in_list = connection_->prepareStatement(request_string);
        int variable_count = 1;
        for (const auto& player : players_)
        {
            //get each id
            sql::PreparedStatement* user_id_query = connection_->prepareStatement("SELECT playerID FROM players WHERE name = ?");
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
        all_players_in_list->setInt(variable_count, static_cast<int>(players_.size()));
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
int DatabaseAPI::getUserId(const std::string &username) const
{
    sql::PreparedStatement* user_id_query = connection_->prepareStatement("SELECT playerID FROM players WHERE name = ?");
    user_id_query->setString(1, to_upper(username));
    sql::ResultSet* res = user_id_query->executeQuery();
    if (res->next())
    {
        const int id = res->getInt(1);
        delete user_id_query;
        delete res;
        return id;
    }
    delete user_id_query;
    delete res;
    return -1;
}
std::unordered_map<std::string, int> DatabaseAPI::getAllUserIds() const
{
    std::unordered_map<std::string, int> result;
    sql::Statement* userQuery = connection_->createStatement();
    sql::ResultSet* res = userQuery->executeQuery("SELECT playerID, name FROM players");
    while (res->next())
    {
        const int id = res->getInt(1);
        const std::string name = res->getString(2);
        result[name] = id;
    }
    delete userQuery;
    delete res;
    return result;
}

std::string DatabaseAPI::getUserName(int id) const
{
    sql::PreparedStatement* get_name_query = connection_->prepareStatement("SELECT name FROM players WHERE playerID = ?");
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
void DatabaseAPI::createUser(const std::string& username) const
{
    std::cout << "slm >: creating new user..." << std::endl;
    try
    {
        sql::PreparedStatement* prepared_statement = connection_->prepareStatement("SELECT * FROM players WHERE name = ?");
        prepared_statement->setString(1, to_upper(username));
        sql::ResultSet* result_set = prepared_statement->executeQuery();
        if (result_set->next())
        {
            std::cout << "slm >: user already exists!" << std::endl;
        } else
        {
            sql::PreparedStatement* create_user_statement = connection_->prepareStatement("INSERT INTO players(name) VALUES (?)");
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

void DatabaseAPI::loginUser(const std::string& username)
{
    if (username.empty() || std::ranges::find(players_, username) != players_.end())
    {
        std::cout << "slm >: user already exists or invalid name" << std::endl;
    }
    else
    {
        players_.push_back(username);
    }
}

void DatabaseAPI::logoutUser(const std::string& username)
{
    players_.erase(std::ranges::remove(players_, username).begin(), players_.end());
}
void DatabaseAPI::clearUsers()
{
    players_.clear();
}


void DatabaseAPI::showLoggedInUsers() const
{
    std::cout << "slm >: logged in users are:";
    for (const auto& player : players_)
    {
        std::cout << " " + player;
    }
    std::cout << std::endl;
}

void DatabaseAPI::setListId()
{
    if (int id = getTableId(); id != 0)
    {
        currentListId_ = id;
        //std::cout << id << std::endl;
        return;
    }
    else
    {
        //create list;
    }
}


void DatabaseAPI::changeList()
{
    if (!players_.empty())
    {
        std::cout << "slm >: the current list in use contains the players:";
        for (const auto& player : players_)
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
            for (const auto& username : usernames)
            {
                loginUser(to_upper(username));
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

void DatabaseAPI::pushEntry(const GameEntry& entry) const
{
    try
    {
        //get id
        sql::PreparedStatement* get_id_query = connection_->prepareStatement(
            "SELECT gameID FROM possibleGames WHERE score = ? AND typeName = ? AND peaks = ? AND modifierName = ?");
        get_id_query->setInt(1,calculate_game_value(entry));
        get_id_query->setString(2, return_type_name(entry.type));
        get_id_query->setInt(3, entry.peaks);
        get_id_query->setString(4, entry.modifier.name);
        sql::ResultSet* result_set = get_id_query->executeQuery();

        if (result_set->next())
        {
            int id = result_set->getInt(1);
            //std::cout << "Game found with id: " << id << std::endl;
            std::string query = "INSERT INTO played_games(playedGameID, playerID, listID, hasWon, playedAt) VALUES (?,?,?,?, CURRENT_TIMESTAMP)";
            sql::PreparedStatement* insert_query = connection_->prepareStatement(query);
            insert_query->setInt(1, id);
            insert_query->setInt(2, entry.user_id);
            insert_query->setInt(3, currentListId_);
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
bool DatabaseAPI::publishEntry(const GameEntry& entry) const
{
    int id = -1;
    sql::PreparedStatement* get_id_query = connection_->prepareStatement(
            "SELECT gameID FROM possibleGames WHERE score = ? AND typeName = ? AND peaks = ? AND modifierName = ?");
    get_id_query->setInt(1,calculate_game_value(entry));
    get_id_query->setString(2, return_type_name(entry.type));
    get_id_query->setInt(3, entry.peaks);
    get_id_query->setString(4, entry.modifier.name);
    sql::ResultSet* result_set = get_id_query->executeQuery();
    if (result_set->next())
    {
        id = result_set->getInt(1);
    }
    else return false;
    std::string query = "INSERT INTO played_games(playedGameID, playerID, listID, hasWon, playedAt) VALUES (?,?,?,?, CURRENT_TIMESTAMP)";
    sql::PreparedStatement* publishQuery = connection_->prepareStatement(query);
    publishQuery->setInt(1, id);
    publishQuery->setInt(2, entry.user_id);
    publishQuery->setInt(3, currentListId_);
    publishQuery->setBoolean(4, entry.won);
    publishQuery->executeQuery();
    delete publishQuery;
    return true;
}


int DatabaseAPI::returnGameValue(const GameEntry& entry) const
{
    std::string query = "SELECT score FROM possibleGames WHERE score = ? AND typeName = ? AND peaks = ? AND modifierName = ?";
    sql::PreparedStatement* get_id_query = connection_->prepareStatement(query);
    get_id_query->setInt(1,calculate_game_value(entry));
    get_id_query->setString(2, return_type_name(entry.type));
    get_id_query->setInt(3, entry.peaks);
    get_id_query->setString(4, entry.modifier.name);
    sql::ResultSet* result_set = get_id_query->executeQuery();
    delete get_id_query;
    if (result_set->next())
    {
        const int score = result_set->getInt(1);
        delete result_set;
        return score;
    }
    delete result_set;
    return -1;

}


std::vector<std::string> DatabaseAPI::getEntriesString() const
{
    std::vector<std::string> entries;
    try
    {
        std::string query = "SELECT players.name, possibleGames.typeName, possibleGames.peaks, possibleGames.modifierName, "
        "possibleGames.score, played_games.playedAt, played_games.hasWon FROM played_games JOIN players ON players.playerID = "
        "played_games.playerID JOIN possibleGames ON played_games.playedGameID = possibleGames.gameID WHERE listID = ?"
        "ORDER BY played_games.gameID DESC LIMIT 10";
        sql::PreparedStatement* get_games_query = connection_->prepareStatement(query);
        get_games_query->setInt(1, currentListId_);
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

std::vector<TableGameEntry> DatabaseAPI::getEntries(const int limit, const int offset) const
{
    std::vector<TableGameEntry> entries;
    try
    {
        std::string query = "SELECT players.name, played_games.hasWon, possibleGames.typeName, possibleGames.peaks, "
                            "possibleGames.modifierName, possibleGames.score, played_games.playedAt, played_games.gameID "
                            "FROM played_games JOIN possibleGames ON played_games.playedGameID = possibleGames.gameID "
                            "JOIN players ON played_games.playerID = players.playerID WHERE played_games.listID = ? "
                            "AND played_games.gameID < ? ORDER BY played_games.gameID DESC LIMIT ?";
        sql::PreparedStatement* get_games_query = connection_->prepareStatement(query);
        get_games_query->setInt(1, currentListId_);
        get_games_query->setInt(2, offset);
        get_games_query->setInt(3, limit);
        sql::ResultSet* result_set = get_games_query->executeQuery();
        while (result_set->next())
        {
            TableGameEntry entry;
            entry.playerName = result_set->getString(1);
            entry.hasWon = result_set->getBoolean(2);
            entry.GameType = result_set->getString(3);
            entry.peaks = result_set->getInt(4);
            entry.ModifierName = result_set->getString(5);
            entry.value = result_set->getInt(6);
            entry.dateTime = result_set->getString(7);
            entry.id = result_set->getInt(8);
            entries.push_back(entry);
        }
        delete result_set;
        delete get_games_query;
    } catch(sql::SQLException& e)
    {
        std::cout << "slm >: error occurred: " << e.what() << std::endl;
    }
    return entries;
}

int DatabaseAPI::getListId() const
{
    return currentListId_;
}
std::vector<std::string> DatabaseAPI::getUsers() const
{
    return players_;
}






bool isAlpha(std::string const &str) {
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") ==
        std::string::npos;
}

bool validate_names(const std::vector<std::string>& players)
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


std::string to_upper(const std::string &str)
{
    std::string new_str = str;
    std::ranges::transform(new_str,new_str.begin(), ::toupper);
    return new_str;
}

//only to be invoked after name validity was checked
std::string getTableName(const std::vector<std::string>& players)
{
    //make uppercase
    std::vector<std::string> sorted_player_names;
    std::string table_name;
    for (const auto& name: players)
    {
        sorted_player_names.push_back(to_upper(name));
    }

    std::ranges::sort(sorted_player_names);
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