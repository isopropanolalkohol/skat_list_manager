//
// Created by joshua on 29.08.25.
//

#include "central_view.h"
#include "entries_table_model.h"
#include <QWidget>
#include <QTableView>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QToolButton>
#include <QButtonGroup>

CentralView::CentralView(DataService& ds, QWidget* parent) : QWidget(parent), ds_(ds)
{
    split_ = new QSplitter(Qt::Horizontal, this);

    table_ = new QTableView(split_);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setSortingEnabled(true);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->verticalHeader()->setVisible(true);

    rightSplit_ = new QSplitter(Qt::Vertical, split_);

    boxActions_ = new QGroupBox(tr("Aktionen"), rightSplit_);
    boxStats_ = new PlayerStats(ds_, rightSplit_);
    boxPlayedGamesStatistics_ = new QGroupBox(tr("Mini-Statistiken"), rightSplit_);



    auto* layout2 = new QVBoxLayout(boxPlayedGamesStatistics_);
    layout2->addStretch();
    layout2->setContentsMargins(12,12,12,12);

    auto* actionLayout = new QGridLayout(boxActions_);
    actionLayout->setContentsMargins(12,12,12,12);
    actionLayout->setHorizontalSpacing(8);
    actionLayout->setVerticalSpacing(8);
    actionLayout->setAlignment(Qt::AlignCenter);
    actionLayout->setColumnStretch(0,1);
    actionLayout->setColumnStretch(1,1);
    actionLayout->setColumnStretch(2,1);
    actionLayout->setRowStretch(0,1);
    actionLayout->setRowStretch(1,1);

    //add entry button
    auto* addEntryButton = new QToolButton(boxActions_);
    addEntryButton->setText(tr("Eintrag hinzufügen"));
    addEntryButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addEntryButton->setIcon(QIcon(":/icons/addEntry.svg"));
    addEntryButton->setIconSize(QSize(24,24));
    addEntryButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    actionsGroup_ = new QButtonGroup(this);
    actionsGroup_->setExclusive(false);
    actionsGroup_->addButton(addEntryButton, static_cast<int>(Action::Add));
    actionLayout->addWidget(addEntryButton, 0, 0, Qt::AlignCenter);

    connect(actionsGroup_, &QButtonGroup::idClicked, this,
            [this](int id){ emit actionTriggered(static_cast<Action>(id)); });
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
    boxStats_->reload();
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


