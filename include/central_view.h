//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QWidget>
#include <QSplitter>
#include <QGroupBox>

#include "player_stats.h"

class QTableView;
class EntriesTableModel;

class CentralView : public QWidget
{
    Q_OBJECT
    public:
    explicit CentralView(DataService& ds,QWidget *parent = nullptr);
    void setModel(EntriesTableModel *model);
    void refresh() const;
    void prefetchIfNeeded();
    void updateDetailsFromSelection();

    private:
    DataService& ds_;

    QTableView *table_ = nullptr;
    EntriesTableModel *model_ = nullptr;
    QSplitter* split_ = nullptr;
    QSplitter* rightSplit_ = nullptr;
    PlayerStats* boxStats_ = nullptr;
    QGroupBox* boxPlayedGamesStatistics_ = nullptr;
    QGroupBox* boxActions_ = nullptr;
};