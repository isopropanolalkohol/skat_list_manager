//
// Created by joshua on 29.08.25.
//

#pragma once

#include "database_api.h"
#include "skat.h"
#include <QString>

class DataService
{
    public:
    explicit DataService(DatabaseAPI& db);

    [[nodiscard]] std::vector<TableGameEntry> getEntries(int limit, int offset) const;
    QStringList getPlayerNames();
    void loginUsers(std::vector<std::string> players);
    void rollback() const;
    void commit() const;
    void begin() const;

    private:
    DatabaseAPI& db_;
};
