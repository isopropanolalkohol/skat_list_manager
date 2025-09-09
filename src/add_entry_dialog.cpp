//
// Created by joshua on 29.08.25.
//
#include "add_entry_dialog.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>

AddEntryDialog::AddEntryDialog(DataService& ds, QWidget* parent) : QDialog(parent), ds_(ds)
{
    playerChoiceBox_ = new QComboBox(this);
    typeBox_ = new QComboBox(this);
    peaksChoiceBox_ = new QComboBox(this);
    modifierChoiceBox_ = new QComboBox(this);
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,this);

    auto *form = new QFormLayout(this);
    form->addRow(tr("Spieler"), playerChoiceBox_);
    form->addRow(tr("Spiel"), typeBox_);
    form->addRow(tr("Spitzen"), peaksChoiceBox_);
    form->addRow(tr("Modifier"), modifierChoiceBox_);
    form->addRow(buttons_);

    connect(buttons_, &QDialogButtonBox::accepted, this, &AddEntryDialog::accept);
    connect(buttons_, &QDialogButtonBox::rejected, this, &AddEntryDialog::reject);
    auto upd = [this]{ updateOkEnabled(); };
    connect(playerChoiceBox_, &QComboBox::currentIndexChanged, this, upd);
    connect(typeBox_,&QComboBox::currentIndexChanged, this, upd);
    updateOkEnabled();
}

void AddEntryDialog::setPlayers(const QList<QString> &playerNames)
{

}

void AddEntryDialog::updateOkEnabled()
{
    const bool ready = playerChoiceBox_->currentIndex() >= 0 && typeBox_->currentIndex() >= 0;
    buttons_->button(QDialogButtonBox::Ok)->setEnabled(ready);
}
