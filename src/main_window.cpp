//
// Created by joshua on 28.08.25.
//
#include "main_window.h"
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Skat-Listen-Manager");
    resize(1100, 700);

    createActions();
    buildMenus();
    buildStatusBar();
    connectActions();

    auto* central = new QWidget(this);
    central->setObjectName("central");
    setCentralWidget(central);
    refreshTable();
}

void MainWindow::createActions()
{
    actOpen = new QAction(tr("Öffnen..."),this);
    actOpen->setShortcut(QKeySequence::Open);

    actClose = new QAction(tr("Schließen"), this);
    actClose->setShortcut(QKeySequence::Close);
    actClose->setEnabled(false);

    actQuit  = new QAction(tr("Beenden"), this);
    actQuit->setShortcut(QKeySequence::Quit);

    actAbout = new QAction(tr("Über…"), this);

    actEditCredentials = new QAction(tr("Einstellungen"), this);

    actEditPlayers = new QAction(tr("Spieler"), this);

}

void MainWindow::buildMenus()
{
    menuFile = menuBar()->addMenu(tr("Datei"));
    menuFile->addAction(actOpen);
    menuFile->addAction(actClose);
    menuFile->addSeparator();
    menuFile->addAction(actQuit);

    menuDatabase = menuBar()->addMenu(tr("Datenbank"));
    menuDatabase->addAction(actEditCredentials);
    menuDatabase->addAction(actEditPlayers);

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
}

void MainWindow::onOpen()
{
    /* TODO: QFileDialog */
}
void MainWindow::onClose()
{
    /* TODO: CentralView reset; actClose->setEnabled(false); */
}
void MainWindow::onQuit()
{
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

}


void MainWindow::refreshTable()
{
    auto rows = nullptr;
}