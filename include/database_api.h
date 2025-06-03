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
    DatabaseAPI();
    ~DatabaseAPI();

    void start_transaction() const;
    void commit_transaction() const;
    void rollback_transaction() const;

    //the skat database options:
    void create_users_if_dont_exist() const;
    void create_new_table() const;
    void delete_table() const;
    std::unordered_map<std::string, int> get_points() const;
    int get_table_id() const;
    int get_user_id(std::string username) const;
    std::string get_user_name(int id) const;
    void create_user(std::string username) const;
    void delete_user(std::string username) const;

    void login_user(const std::string& username);
    void logout_user(std::string username);
    void clear_users();
    void show_logged_in_users() const;


    void set_list_id();
    void change_list();
    void push_entry(GameEntry entry) const;
    std::vector<std::string> get_entries() const;

//getters
    int get_list_id() const;
    std::vector<std::string> get_users() const;
    private:
    sql::Connection* connection;
    std::vector<std::string> players;
    int current_list_id;
};





std::string to_upper(std::string str);
std::string generate_tuple_with_question_mark(const int count);
bool isAlpha(std::string const &str);
bool validate_names(std::vector<std::string> players);
std::string getTableName(std::vector<std::string> players);

#endif //DATABASE_API_H
