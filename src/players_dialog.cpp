//
// Created by joshua on 29.08.25.
//


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>   // für button(...)
#include <QSizePolicy>

#include "players_dialog.h"
#include "main_window.h"

PlayersDialog::PlayersDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Spieler festlegen"));
    setModal(true);

    setMinimumSize(420, 300);
    resize(520, 380);

    // 1) Layout gehört dem Dialog
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // 2) Label
    auto* lbl = new QLabel(tr("Ein Name pro Zeile"), this);
    lbl->setWordWrap(true);
    layout->addWidget(lbl);

    // 3) Textfeld füllt Platz
    namesEdit_ = new QPlainTextEdit(this);
    namesEdit_->setPlaceholderText(tr("Anna\nBen\nChris"));
    namesEdit_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    namesEdit_->setMinimumHeight(140);
    layout->addWidget(namesEdit_, /*stretch*/ 1);

    // 4) ButtonBox NICHT expandieren lassen
    buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox_->setOrientation(Qt::Horizontal);
    buttonBox_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 5) Eigene Button-Zeile mit Stretch nach links
    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(buttonBox_);
    layout->addLayout(btnRow);

    // 6) Standard-Connects
    connect(buttonBox_, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox_, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // 7) OK erst aktivieren, wenn mind. 1 Name
    auto* okBtn = buttonBox_->button(QDialogButtonBox::Ok);
    okBtn->setEnabled(false);
    okBtn->setDefault(true);

    auto updateOk = [this, okBtn]{
        const auto lines = namesEdit_->toPlainText().split('\n');
        bool hasName = false;
        for (const auto& ql : lines) {
            if (!ql.trimmed().isEmpty()) { hasName = true; break; }
        }
        okBtn->setEnabled(hasName);
    };
    connect(namesEdit_, &QPlainTextEdit::textChanged, this, updateOk);
    updateOk();

    namesEdit_->setFocus();
    setSizeGripEnabled(true);

}

std::vector<std::string> PlayersDialog::result() const
{
    std::vector<std::string> out;
    const auto lines = namesEdit_->toPlainText().split('\n');
    out.reserve(lines.size());
    for (const auto& ql : lines)
    {
        const auto s = ql.trimmed();
        if (!s.isEmpty())
            out.emplace_back(s.toUtf8().toStdString());
    }
    return out;
}

// hier weitermachen bei players dialog