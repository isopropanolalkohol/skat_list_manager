//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QWidget>

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

    private:
    QTableView *table_ = nullptr;
    EntriesTableModel *model_ = nullptr;
};