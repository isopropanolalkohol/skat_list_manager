//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QDialog>
#include <QString>

class QLineEdit;
class QComboBox;
class QDialogButtonBox;
class QSpinBox;

struct GameEntry;

class AddEntryDialog : public QDialog
{
    Q_OBJECT
    public:
    explicit AddEntryDialog(QWidget *parent = nullptr);

    void setPlayers(const QList<QString> &playerNames);


    GameEntry result() const;

    void setTypes(const QStringList &types);

    private:
    QComboBox *playerChoiceBox_ = nullptr;
    QComboBox *typeBox_ = nullptr;
    QComboBox *peaksChoiceBox_ = nullptr;
    QComboBox *modifierChoiceBox_ = nullptr;
    QDialogButtonBox *buttons_ = nullptr;

    void updateOkEnabled();
};