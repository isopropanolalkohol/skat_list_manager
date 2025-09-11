//
// Created by joshua on 05.09.25.
//

#include "player_stats.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

PlayerStats::PlayerStats(DataService& ds, QWidget *parent) : QWidget(parent), ds_(ds)
{
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(12,12,12,12);
    main->setSpacing(8);

    rowsLay_ = new QVBoxLayout();
    rowsLay_->setSpacing(6);

    main->addLayout(rowsLay_);
    main->addStretch();
    reload();
}

void PlayerStats::reload()
{
    setUpdatesEnabled(false);

    while (auto* item = rowsLay_->takeAt(0))
    {
        if (auto* w = item->widget()) w->deleteLater();
        delete item;
    }
    std::vector<PlayerPoint> stats = ds_.getPoints();
    std::ranges::reverse(stats);
    for (const auto &p : stats)
    {
        auto* row = new PlayerRowWidget(this);
        row->setData(p);
        rowsLay_->addWidget(row);
    }

    setUpdatesEnabled(true);
}

PlayerRowWidget::PlayerRowWidget(QWidget *parent)
{
    auto* row = new QHBoxLayout(this);
    row->setContentsMargins(8,6,8,6);
    row->setSpacing(8);

    name_ = new QLabel(this);
    name_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    score_ = new QLabel(this);
    score_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    score_->setMinimumWidth(64);

    row->addWidget(name_);
    row->addWidget(score_);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(QFontMetrics(font()).lineSpacing() + 10);
}

void PlayerRowWidget::setData(const PlayerPoint &p)
{
    name_->setText(QString::fromUtf8(p.name.c_str()));
    score_->setText(QString::number(p.score));
}

