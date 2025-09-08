//
// Created by joshua on 28.08.25.
//
#include "main_window.h"
#include <QWidget>
#include <QTimer>
#include <QMessageBox>

#include "players_dialog.h"

MainWindow::MainWindow(DataService& ds, QWidget* parent) : QMainWindow(parent), service_(ds) {
    setWindowTitle("Skat-Listen-Manager");
    resize(1100, 700);

    createActions();
    buildMenus();
    buildStatusBar();
    connectActions();
    model_   = new EntriesTableModel(service_, this);
    central_ = new CentralView(this);
    central_->setModel(model_);
    setCentralWidget(central_);
    QTimer::singleShot(0, this, &MainWindow::onEditPlayers);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!maybeSave())
        {
        event->ignore();
        return;
    }
    event->accept();
}


void MainWindow::createActions()
{
    actOpen = new QAction(tr("Öffnen..."),this);
    actOpen->setShortcut(QKeySequence::Open);

    actClose = new QAction(tr("Schließen"), this);
    actClose->setShortcut(QKeySequence::Close);
    actClose->setEnabled(false);

    actSave = new QAction(tr("Speichern"), this);
    actSave->setShortcut(QKeySequence::Save);

    actQuit  = new QAction(tr("Beenden"), this);
    actQuit->setShortcut(QKeySequence::Quit);

    actAbout = new QAction(tr("Über…"), this);

    actEditCredentials = new QAction(tr("Einstellungen"), this);

    actEditPlayers = new QAction(tr("Spieler"), this);

    actDebugDirty = new QAction(tr("debug->dirty"), this);

}

void MainWindow::buildMenus()
{
    menuFile = menuBar()->addMenu(tr("Datei"));
    menuFile->addAction(actOpen);
    menuFile->addAction(actClose);
    menuFile->addSeparator();
    menuFile->addAction(actQuit);
    menuFile->addAction(actSave);

    menuDatabase = menuBar()->addMenu(tr("Datenbank"));
    menuDatabase->addAction(actEditCredentials);
    menuDatabase->addAction(actEditPlayers);
    menuDatabase->addAction(actDebugDirty);

    menuHelp = menuBar()->addMenu(tr("Hilfe"));
    menuHelp->addAction(actAbout);
}

void MainWindow::buildStatusBar()
{
    statusBar()->showMessage("Bereit");
}

void MainWindow::connectActions()
{
    connect(actOpen,  &QAction::triggered, this, &MainWindow::onOpen);
    connect(actClose, &QAction::triggered, this, &MainWindow::onClose);
    connect(actQuit,  &QAction::triggered, this, &MainWindow::onQuit);
    connect(actAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(actEditCredentials, &QAction::triggered, this, &MainWindow::onEditCredentials);
    connect(actEditPlayers, &QAction::triggered, this, &MainWindow::onEditPlayers);
    connect(actSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(actDebugDirty, &QAction::triggered, this, &MainWindow::markDirty);
}

void MainWindow::onOpen()
{
    /* TODO: QFileDialog */
}
void MainWindow::onClose()
{
    /* TODO: CentralView reset; actClose->setEnabled(false); */
}

void MainWindow::onSave()
{
    service_.commit();
    service_.begin();
    statusBar()->showMessage("Gespeichert...", 2000);
}


void MainWindow::onQuit()
{
    // edit save options
    close();
}            // oder QApplication::quit()
void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("Über"), tr("Skat List Manager"));
}

void MainWindow::onEditCredentials()
{
    // TODO: Enter flight reacts gif
}
void MainWindow::onEditPlayers()
{
    if (PlayersDialog dlg(this); dlg.exec() == QDialog::Accepted)
    {
        std::vector<std::string> players = dlg.result();
        if (players.size() > 2)
        {
            service_.loginUsers(players);
            model_->setReady(true);
        }
    }
    central_->refresh();
    model_->reload();
    QTimer::singleShot(0, central_, &CentralView::prefetchIfNeeded);
}


void MainWindow::refreshTable()
{
    auto rows = nullptr;
}

void MainWindow::markClean()
{
    dirty_ = false;
}

void MainWindow::markDirty()
{
    dirty_ = true;
}

bool MainWindow::maybeSave()
{
    if (!dirty_) return true;
    auto ret  = QMessageBox::question(this, tr("Änderungen speichern?"), tr("Es gibt ungespeicherte Änderungen"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
    if (ret == QMessageBox::Save)
    {
        service_.commit();
        service_.begin();
        return true;
    }
    if (ret == QMessageBox::Discard) return true;
    return false;
}

