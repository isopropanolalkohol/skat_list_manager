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
    beginResetModel();
    rows_ = ds_.getEntries();
    endResetModel();
}