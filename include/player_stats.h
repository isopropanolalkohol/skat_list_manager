//
// Created by joshua on 05.09.25.
//

#pragma once

#include <QLabel>
#include <QWidget>

#include "data_service.h"

class QVBoxLayout;

struct PlayerPoint
{
    std::string name;
    int score;
};


class PlayerStats : public QWidget
{
    Q_OBJECT

    public:
    explicit PlayerStats(DataService& ds, QWidget *parent = nullptr);

    void reload();

    private:
    QVBoxLayout *rowsLay_ = nullptr;
    DataService& ds_;
};


class PlayerRowWidget : public QWidget
{
    Q_OBJECT

    public:
    explicit PlayerRowWidget(QWidget *parent = nullptr);

    void setData(const PlayerPoint &p);


    private:
    QLabel *name_ = nullptr;
    QLabel *score_ = nullptr;
};
