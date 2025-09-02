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

    [[nodiscard]] std::vector<TableGameEntry> getEntries() const;
    QStringList getPlayerNames();

    private:
    DatabaseAPI& db_;
};
