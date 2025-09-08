//
// Created by joshua on 29.08.25.
//

#include "data_service.h"
#include "player_stats.h"
#include <qlist.h>
#include <QString>

DataService::DataService(DatabaseAPI &db) : db_(db){}

std::vector<TableGameEntry> DataService::getEntries(const int limit, const int offset) const
{
    return db_.getEntries(limit, offset);
}

QStringList DataService::getPlayerNames()
{
    QStringList pnames;
    return pnames;
}

void DataService::loginUsers(const std::vector<std::string> players)
{
    db_.clearUsers();
    for (const auto& player : players)
    {
        db_.loginUser(to_upper(player));
    }
    if (players.size() < 3) return;
    db_.createNewTable();
    db_.setListId();
}

void DataService::begin() const
{
    db_.startTransaction();
}
void DataService::commit() const
{
    db_.commitTransaction();
}
void DataService::rollback() const
{
    db_.rollbackTransaction();
}

std::vector<PlayerPoint> DataService::getPoints() const
{
    std::vector<PlayerPoint> points;
    const std::unordered_map<std::string, int> result = db_.getPoints();
    points.reserve(result.size());
    for (const auto& entry : result)
    {
        PlayerPoint point = {entry.first, entry.second};
        points.emplace_back(point);
    }
    std::sort(points.begin(), points.end(), [](const PlayerPoint &a, const PlayerPoint &b){return a.score < b.score;});
    return points;
}

