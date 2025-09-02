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
CentralView::CentralView(QWidget* parent) : QWidget(parent)
{
    table_ = new QTableView(this);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setSortingEnabled(true);
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->verticalHeader()->setVisible(true);

    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(table_);
}

void CentralView::setModel(EntriesTableModel *model)
{
    model_ = model;
    table_->setModel(model_);
}

void CentralView::refresh()
{
    if (model_) model_->reload();
}