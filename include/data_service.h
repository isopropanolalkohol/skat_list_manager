//
// Created by joshua on 29.08.25.
//

#pragma once

#include "database_api.h"
#include "skat.h"
#include <QString>
class PlayerPoint;

class DataService
{
    public:
    explicit DataService(DatabaseAPI& db);

    [[nodiscard]] std::vector<TableGameEntry> getEntries(int limit, int offset) const;
    QStringList getPlayerNames();
    std::vector<std::string> players();
    [[nodiscard]] std::vector<PlayerPoint> getPoints() const;
    void loginUsers(std::vector<std::string> players);
    void rollback() const;
    void commit() const;
    void begin() const;
    int getUserID(std::string username) const;
    int returnGameValue(GameEntry entry) const;
    bool publish(GameEntry entry) const;

    private:
    DatabaseAPI& db_;
    std::unordered_map<std::string, int> playerIDs_;
    std::vector<std::string> loggedInPlayerNames_;
};
