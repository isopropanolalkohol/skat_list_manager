//
// Created by joshua on 29.08.25.
//
#include "add_entry_dialog.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSignalBlocker>
#include "skat.h"

AddEntryDialog::AddEntryDialog(DataService& ds, QWidget* parent) : QDialog(parent), ds_(ds)
{
    playerChoiceBox_ = new QComboBox(this);
    typeBox_ = new QComboBox(this);
    peaksChoiceBox_ = new QComboBox(this);
    modifierChoiceBox_ = new QComboBox(this);
    wonChoiceBox_ = new QComboBox(this);
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,this);

    auto *main = new QVBoxLayout(this);
    auto *form = new QFormLayout();
    setMinimumSize(760,520);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    form->addRow(tr("Spieler"), playerChoiceBox_);
    form->addRow(tr("Spiel"), typeBox_);
    form->addRow(tr("Spitzen"), peaksChoiceBox_);
    form->addRow(tr("Modifier"), modifierChoiceBox_);
    form->addRow(tr("Gewonnen?"), wonChoiceBox_);
    form->addRow(buttons_);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    main->addLayout(form);
    main->addWidget(buttons_);
    main->setStretch(0,1);
    main->setStretch(1,0);

    connect(buttons_, &QDialogButtonBox::accepted, this, &AddEntryDialog::onAccept);
    connect(buttons_, &QDialogButtonBox::rejected, this, &AddEntryDialog::reject);
    auto upd = [this]{ updateOkEnabled(); };
    connect(playerChoiceBox_, &QComboBox::currentIndexChanged, this, upd);
    connect(typeBox_,&QComboBox::currentIndexChanged, this, upd);
    updateOkEnabled();

    playerChoiceBox_->setMinimumWidth(260);
    playerChoiceBox_->setMinimumContentsLength(18);
    playerChoiceBox_->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

    playerChoiceBox_->setMaxVisibleItems(12);
    playerChoiceBox_->setPlaceholderText(tr("Bitte auswählen…"));
    playerChoiceBox_->setEditable(false);
    playerChoiceBox_->setInsertPolicy(QComboBox::NoInsert);

    typeBox_->setMinimumWidth(260);
    typeBox_->setMinimumContentsLength(18);
    typeBox_->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

    typeBox_->setMaxVisibleItems(12);
    typeBox_->setPlaceholderText(tr("Bitte auswählen…"));
    typeBox_->setEditable(false);
    typeBox_->setInsertPolicy(QComboBox::NoInsert);

    peaksChoiceBox_->setMinimumWidth(260);
    peaksChoiceBox_->setMinimumContentsLength(18);
    peaksChoiceBox_->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

    peaksChoiceBox_->setMaxVisibleItems(12);
    peaksChoiceBox_->setPlaceholderText(tr("Bitte auswählen…"));
    peaksChoiceBox_->setEditable(false);
    peaksChoiceBox_->setInsertPolicy(QComboBox::NoInsert);

    modifierChoiceBox_->setMinimumWidth(260);
    modifierChoiceBox_->setMinimumContentsLength(18);
    modifierChoiceBox_->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

    modifierChoiceBox_->setMaxVisibleItems(12);
    modifierChoiceBox_->setPlaceholderText(tr("Bitte auswählen…"));
    modifierChoiceBox_->setEditable(false);
    modifierChoiceBox_->setInsertPolicy(QComboBox::NoInsert);

    wonChoiceBox_->setMinimumWidth(260);
    wonChoiceBox_->setMinimumContentsLength(18);
    wonChoiceBox_->setEditable(false);
    wonChoiceBox_->setInsertPolicy(QComboBox::NoInsert);
    wonChoiceBox_->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

    connect(typeBox_, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddEntryDialog::onGameChanged);
    connect(playerChoiceBox_, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddEntryDialog::updateOkEnabled);
    connect(modifierChoiceBox_, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddEntryDialog::updateOkEnabled);
    connect(peaksChoiceBox_, qOverload<int>(&QComboBox::currentIndexChanged), this,  &AddEntryDialog::updateOkEnabled);
    connect(wonChoiceBox_, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddEntryDialog::updateOkEnabled);

    onGameChanged();
    updateOkEnabled();
}

void AddEntryDialog::fillCombos(std::vector<std::string> players)
{
    QSignalBlocker b(playerChoiceBox_);
    playerChoiceBox_->clear();
    playerChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
    for (auto& player : players)
    {
        playerChoiceBox_->addItem(QString::fromUtf8(player.c_str()), ds_.getUserID(player));
    }
    playerChoiceBox_->setCurrentIndex(0);

    QSignalBlocker b1(typeBox_);
    typeBox_->clear();
    typeBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
    for (int i = 0; i < UNSET; i++)
    {
        if (i == ZERO_HAND || i == ZERO_HAND_OUVERT) continue;
        typeBox_->addItem(QString::fromUtf8(return_type_name(static_cast<GameType>(i))), i);
    }
    typeBox_->setCurrentIndex(0);

    QSignalBlocker b3(peaksChoiceBox_);
    peaksChoiceBox_->clear();
    peaksChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
    for (int i = 1; i < 12; i++)
    {
        peaksChoiceBox_->addItem(QString::number(i), i); //continue here
    }
    peaksChoiceBox_->setCurrentIndex(0);

    QSignalBlocker b4(modifierChoiceBox_);
    modifierChoiceBox_->clear();
    modifierChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());

    for (int i = 0; i < modifiers.size(); i++)
    {
        modifierChoiceBox_->addItem(QString::fromUtf8(modifiers[i].name.c_str()), i);
    }
    modifierChoiceBox_->setCurrentIndex(0);

    QSignalBlocker b5(wonChoiceBox_);
    wonChoiceBox_->clear();
    wonChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
    wonChoiceBox_->addItem(QString::fromUtf8("Gewonnen"), 1);
    wonChoiceBox_->addItem(QString::fromUtf8("Verloren"), 0);
    wonChoiceBox_->setCurrentIndex(0);
}

void AddEntryDialog::updateOkEnabled()
{
    const bool ready = isValidOrDisabled(playerChoiceBox_) && isValidOrDisabled(modifierChoiceBox_) &&
        isValidOrDisabled(peaksChoiceBox_) && isValidOrDisabled(typeBox_) && isValidOrDisabled(wonChoiceBox_);
    buttons_->button(QDialogButtonBox::Ok)->setEnabled(ready);
}

void AddEntryDialog::onGameChanged()
{
    if (!typeBox_->currentData().isValid()) return;
    const int gameType = typeBox_->currentData().toInt();

    if (gameType == ZERO || gameType == ZERO_OUVERT)
    {
        {
            QSignalBlocker b1(peaksChoiceBox_);
            peaksChoiceBox_->clear();
            peaksChoiceBox_->addItem(tr("- keine Spitzen -"), QVariant());
            peaksChoiceBox_->setCurrentIndex(0);
        }
        peaksChoiceBox_->setEnabled(false);

        {
            QSignalBlocker b2(modifierChoiceBox_);
            modifierChoiceBox_->clear();
            modifierChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
            modifierChoiceBox_->addItem(QString::fromUtf8(modifiers[0].name.c_str()), 0);
            modifierChoiceBox_->addItem(QString::fromUtf8(modifiers[6].name.c_str()), 6);
            modifierChoiceBox_->setCurrentIndex(0);
        }
        modifierChoiceBox_->setEnabled(true);
        {
            QSignalBlocker b5(wonChoiceBox_);
            wonChoiceBox_->clear();
            wonChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
            wonChoiceBox_->addItem(QString::fromUtf8("Gewonnen"), 1);
            wonChoiceBox_->addItem(QString::fromUtf8("Verloren"), 0);
            wonChoiceBox_->setCurrentIndex(0);
        }
        wonChoiceBox_->setEnabled(true);
    }
    else if (gameType == RAMSCH)
    {
        {
            QSignalBlocker b1(peaksChoiceBox_);
            peaksChoiceBox_->clear();
            peaksChoiceBox_->addItem(tr("- keine Spitzen -"), QVariant());
            peaksChoiceBox_->setCurrentIndex(0);
        }
        peaksChoiceBox_->setEnabled(false);

        {
            QSignalBlocker b2(modifierChoiceBox_);
            modifierChoiceBox_->clear();
            modifierChoiceBox_->addItem(tr("- keine Modifier -"), QVariant());
            modifierChoiceBox_->setCurrentIndex(0);
        }
        modifierChoiceBox_->setEnabled(false);

        {
            QSignalBlocker b3(wonChoiceBox_);
            wonChoiceBox_->clear();
            wonChoiceBox_->addItem(tr("Verloren -"), QVariant());
            wonChoiceBox_->setCurrentIndex(0);
        }
        wonChoiceBox_->setEnabled(false);
    }

    else
    {
        {
            // uuuhhh oohhh magic numbers
            QSignalBlocker b1(peaksChoiceBox_);
            peaksChoiceBox_->clear();
            peaksChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
            for (int i = 1; i < 5; i++)
            {
                peaksChoiceBox_->addItem(QString::number(i), i);
            }
            if (gameType != GRAND)
            {
                for (int i = 5; i < 12; i++)
                {
                    peaksChoiceBox_->addItem(QString::number(i), i);
                }
            }
            peaksChoiceBox_->setCurrentIndex(0);
        }
        peaksChoiceBox_->setEnabled(true);

        {
            QSignalBlocker b4(modifierChoiceBox_);
            modifierChoiceBox_->clear();
            modifierChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());

            for (int i = 0; i < modifiers.size(); i++)
            {
                modifierChoiceBox_->addItem(QString::fromUtf8(modifiers[i].name.c_str()), i);
            }
            modifierChoiceBox_->setCurrentIndex(0);
        }
        modifierChoiceBox_->setEnabled(true);
        {
            QSignalBlocker b5(wonChoiceBox_);
            wonChoiceBox_->clear();
            wonChoiceBox_->addItem(QObject::tr("- bitte wählen -"), QVariant());
            wonChoiceBox_->addItem(QString::fromUtf8("Gewonnen"), 1);
            wonChoiceBox_->addItem(QString::fromUtf8("Verloren"), 0);
            wonChoiceBox_->setCurrentIndex(0);
        }
        wonChoiceBox_->setEnabled(true);
    }
    updateOkEnabled();
}
bool AddEntryDialog::isValidOrDisabled(const QComboBox *cb)
{
    return !cb->isEnabled() || cb->currentData().isValid();
}

void AddEntryDialog::onAccept()
{
    if (!typeBox_->currentData().isValid()
      || !playerChoiceBox_->currentData().isValid()) {
        QMessageBox::warning(this, tr("Eingabe unvollständig"), tr("Bitte alle Pflichtfelder wählen."));
        return;
      }
    out_.type = static_cast<GameType>(typeBox_->currentData().toInt());
    out_.user_id = playerChoiceBox_->currentData().toInt();
    setBoxesToDefault();
    switch (out_.type)
    {
        case RAMSCH:
            break;
        case ZERO:
            if (out_.modifier == modifiers[6])
            {
                out_.type = ZERO_HAND;
            }
            break;
        case ZERO_OUVERT:
            if (out_.modifier == modifiers[6])
            {
                out_.type = ZERO_HAND_OUVERT;
            }
            break;
        default:
            break;
    }


    if (ds_.returnGameValue(out_) == -1)
    {
        QMessageBox::critical(this, tr("Fehler"), tr("Datenbank hat den Eintrag nicht gefunden!"));
        return;
    }
    accept();
}

GameEntry AddEntryDialog::result() const
{
    return out_;
}

void AddEntryDialog::setBoxesToDefault()
{
    if (!modifierChoiceBox_->currentData().isValid())
    {
        out_.modifier = modifiers[0];
    }
    else out_.modifier = modifiers[modifierChoiceBox_->currentData().toInt()];
    if (!peaksChoiceBox_->currentData().isValid())
    {
        out_.peaks = 0;
    }
    else out_.peaks = peaksChoiceBox_->currentData().toInt();
    if (!wonChoiceBox_->currentData().isValid())
    {
        out_.won = false;
    }
    else out_.won = wonChoiceBox_->currentData().toInt();
    out_.user_id = playerChoiceBox_->currentData().toInt();
    out_.type = static_cast<GameType>(out_.type);
}

