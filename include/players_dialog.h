//
// Created by joshua on 29.08.25.
//

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QPlainTextEdit>

class PlayersDialog : public QDialog
{
    Q_OBJECT
    public:
    explicit PlayersDialog(QWidget *parent = nullptr);

    std::vector<std::string> result() const;
    private:
    QPlainTextEdit* namesEdit_ = nullptr;
    QDialogButtonBox* buttonBox_ = nullptr;

};
