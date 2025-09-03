//
// Created by joshua on 29.08.25.
//
#include "entries_table_model.h"
#include "skat.h" //to stop the compiler from bitching
#include <QVariant>
EntriesTableModel::EntriesTableModel(DataService& ds, QObject *parent) : QAbstractTableModel(parent), ds_(ds){}
int EntriesTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;   // flache Tabelle
    return static_cast<int>(rows_.size());
}

int EntriesTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return ColCount;
}

QVariant EntriesTableModel::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid()) return {};
    const auto r = static_cast<size_t>(idx.row());
    const auto c = static_cast<Column>(idx.column());

    if (role == Qt::DisplayRole) {
        const auto& e = rows_[r];  // GameEntry
        switch (c) {
            case ColName:   return QString::fromUtf8(e.playerName.c_str());
            case ColValue:    return (3*e.hasWon - 2)*e.value;
            case ColGametype: return QString::fromUtf8(e.GameType.c_str());
            case ColPeaks: return e.peaks;
            case ColModifier: return QString::fromUtf8(e.ModifierName.c_str());
            case ColTime:  return QString::fromUtf8(e.dateTime.c_str());
            default: return {};
        }
    }
    if (role == Qt::TextAlignmentRole && c == ColValue)
        return {Qt::AlignRight | Qt::AlignVCenter};

    return {};
}


QVariant EntriesTableModel::headerData(int section, Qt::Orientation o, int role) const
{
    if (o != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section)
    {
        case ColName: return "Spieler";
        case ColValue: return "Punkte";
        case ColGametype: return "Spiel";
        case ColPeaks: return "Spitzen";
        case ColModifier: return "Stufe";
        case ColTime: return "Uhrzeit";
        default: return {};
    }
}

void EntriesTableModel::reload()
{
    if (loading_ || !ready_) return;
    loading_ = true;
    beginResetModel();
    rows_.clear();
    lastId_ = INT_MAX;
    auto first = ds_.getEntries(pageSize_, lastId_);
    rows_.insert(rows_.end(), first.begin(), first.end());
    hasMore_ = (static_cast<int>(first.size()) == pageSize_);
    if (!first.empty()) lastId_ = first.back().id;

    endResetModel();
    loading_ = false;
}

bool EntriesTableModel::canFetchMore(const QModelIndex& parent) const {
    return !parent.isValid() && hasMore_ && !loading_;
}

void EntriesTableModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid() || !hasMore_ || loading_ || !ready_) return;
    loading_ = true;
    auto more = ds_.getEntries(pageSize_,lastId_);
    const int start = static_cast<int>(rows_.size());
    const int count = static_cast<int>(more.size());
    if (count > 0) {
        beginInsertRows(QModelIndex(), start, start + count - 1);
        rows_.insert(rows_.end(), more.begin(), more.end());
        endInsertRows();
    /*
        qDebug() << "[fetchMore] start: rows =" << rows_.size()
         << "hasMore =" << hasMore_ << "loading =" << loading_;*/
        lastId_ = rows_.back().id;
    }
    if (count < pageSize_) hasMore_ = false;
    loading_ = false;
}

void EntriesTableModel::setReady(const bool r)
{
    ready_ = r;
}