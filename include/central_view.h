//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QWidget>
#include <QSplitter>
#include <QGroupBox>

class QTableView;
class EntriesTableModel;

class CentralView : public QWidget
{
    Q_OBJECT
    public:
    explicit CentralView(QWidget *parent = nullptr);
    void setModel(EntriesTableModel *model);
    void refresh() const;
    void prefetchIfNeeded();
    void updateDetailsFromSelection();

    private:
    QTableView *table_ = nullptr;
    EntriesTableModel *model_ = nullptr;
    QSplitter* split_ = nullptr;
    QSplitter* rightSplit_ = nullptr;
    QGroupBox* boxStats_ = nullptr;
    QGroupBox* boxPlayedGamesStatistics_ = nullptr;
    QGroupBox* boxToBeRenamed_ = nullptr;
};