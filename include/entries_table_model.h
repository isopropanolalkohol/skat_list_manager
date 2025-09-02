//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QAbstractTableModel>
#include <vector>
#include <QString>
#include <QObject>

#include "data_service.h"
#include "skat.h"
struct GameEntry;
enum Column
{
    ColName, ColValue, ColGametype, ColPeaks, ColModifier, ColTime, ColCount
};

class EntriesTableModel : public QAbstractTableModel
{
    Q_OBJECT
    public:
    explicit EntriesTableModel(DataService& ds, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void reload();
    void setRows(std::vector<GameEntry> rows);

    private:
    DataService& ds_;
    std::vector<TableGameEntry> rows_;
};
