//
// Created by joshua on 29.08.25.
//

#include "data_service.h"

#include <qlist.h>
#include <QString>

DataService::DataService(DatabaseAPI &db) : db_(db)
{
    // DEBUG: REMOVE
    db.changeList();
    db.createNewTable();
    db.setListId();
}

std::vector<TableGameEntry> DataService::getEntries() const
{
    return db_.getEntries(20);
}

QStringList DataService::getPlayerNames()
{
    QStringList pnames;
    return pnames;
}
