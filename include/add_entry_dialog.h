//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QDialog>
#include <QString>
#include "data_service.h"

class QLineEdit;
class QComboBox;
class QDialogButtonBox;
class QSpinBox;

struct GameEntry;

class AddEntryDialog : public QDialog
{
    Q_OBJECT
    public:
    explicit AddEntryDialog(DataService& ds, QWidget *parent = nullptr);

    void fillCombos(std::vector<std::string> players);

    [[nodiscard]] GameEntry result() const;

    private:
    QComboBox *playerChoiceBox_ = nullptr;
    QComboBox *typeBox_ = nullptr;
    QComboBox *peaksChoiceBox_ = nullptr;
    QComboBox *modifierChoiceBox_ = nullptr;
    QComboBox *wonChoiceBox_ = nullptr;
    QDialogButtonBox *buttons_ = nullptr;

    DataService& ds_;
    GameEntry out_;

    void updateOkEnabled();
    void onGameChanged();
    void onAccept();
    static bool isValidOrDisabled(const QComboBox* cb);
    void setBoxesToDefault();
};