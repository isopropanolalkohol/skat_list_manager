//
// Created by joshua on 04.04.25.
//

#ifndef DATABASE_API_H
#define DATABASE_API_H

#include <mysql_connection.h>
#include "skat.h"

class DatabaseAPI
{
    public:
    explicit DatabaseAPI();
    ~DatabaseAPI();
    DatabaseAPI(const DatabaseAPI&) = delete;
    DatabaseAPI& operator=(const DatabaseAPI&) = delete;

    void startTransaction() const;
    void commitTransaction() const;
    void rollbackTransaction() const;

    //the skat database options:
    void createUsersIfDontExist() const;
    void createNewTable() const;
    void deleteTable() const;
    std::unordered_map<std::string, int> getPoints() const;
    int getTableId() const;
    int getUserId(std::string username) const;
    std::unordered_map<std::string, int> getAllUserIds() const;
    std::string getUserName(int id) const;
    void createUser(std::string username) const;
    void deleteUser(std::string username) const;

    void loginUser(const std::string& username);
    void logoutUser(std::string username);
    void clearUsers();
    void showLoggedInUsers() const;


    void setListId();
    void changeList();
    [[nodiscard]] int returnGameValue(GameEntry entry) const;
    void pushEntry(GameEntry entry) const;
    bool publishEntry(GameEntry entry) const;
    [[nodiscard]] std::vector<std::string> getEntriesString() const;
    [[nodiscard]] std::vector<TableGameEntry> getEntries(int limit, int offset) const;

//getters
    int getListId() const;
    [[nodiscard]] std::vector<std::string> getUsers() const;
    private:
    sql::Connection* connection_;
    std::vector<std::string> players_;
    int currentListId_;
};





std::string to_upper(std::string str);
std::string generate_tuple_with_question_mark(const int count);
bool isAlpha(std::string const &str);
bool validate_names(std::vector<std::string> players);
std::string getTableName(std::vector<std::string> players);

#endif //DATABASE_API_H
