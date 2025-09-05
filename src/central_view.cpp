//
// Created by joshua on 29.08.25.
//

#include "central_view.h"
#include "entries_table_model.h"
#include <QWidget>
#include <QTableView>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTimer>

CentralView::CentralView(QWidget* parent) : QWidget(parent)
{
    split_ = new QSplitter(Qt::Horizontal, this);

    table_ = new QTableView(split_);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setSortingEnabled(true);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->verticalHeader()->setVisible(true);

    rightSplit_ = new QSplitter(Qt::Vertical, split_);

    boxStats_ = new QGroupBox(tr("Gesamtpunkte"), rightSplit_);
    boxPlayedGamesStatistics_ = new QGroupBox(tr("Gespielte Spiele"), rightSplit_);
    boxToBeRenamed_ = new QGroupBox(tr("Baustelle"), rightSplit_);

    auto* layout1 = new QVBoxLayout(boxStats_);
    layout1->addStretch();
    layout1->setContentsMargins(12,12,12,12);
    auto* layout2 = new QVBoxLayout(boxPlayedGamesStatistics_);
    layout2->addStretch();
    layout2->setContentsMargins(12,12,12,12);
    auto* layout3 = new QVBoxLayout(boxToBeRenamed_);
    layout3->addStretch();
    layout3->setContentsMargins(12,12,12,12);



    rightSplit_->setStretchFactor(0,1);
    rightSplit_->setStretchFactor(1,1);
    rightSplit_->setStretchFactor(2,1);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(split_);

    QTimer::singleShot(0, this, [this]{
      split_->setSizes({ 800, 400 });
    });

    split_->setStretchFactor(0,2);
    split_->setStretchFactor(1,1);
}

void CentralView::setModel(EntriesTableModel *model)
{
    model_ = model;
    table_->setModel(model_);
}

void CentralView::refresh() const
{
    if (model_) model_->reload();
    /*qDebug() << "[after reload] rows =" << model_->rowCount()
         << "canFetchMore =" << model_->canFetchMore({});*/
}
void CentralView::prefetchIfNeeded()
{
    if (!model_) return;
    const int vis = table_->viewport()->height()
                  / table_->verticalHeader()->defaultSectionSize();
    while (model_->canFetchMore({}) && model_->rowCount({}) < vis * 2)
    {
        model_->fetchMore({});
    }
    /*
    qDebug() << "[prefetch] rows =" << model_->rowCount()
         << "canFetchMore =" << model_->canFetchMore({});*/
}
