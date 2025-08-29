//
// Created by joshua on 28.08.25.
//

#pragma once

#include <QObject>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
    explicit MainWindow(QWidget* parent = nullptr);

    private:
    void createActions();
    void buildMenus();
    void buildStatusBar();
    void connectActions();

    QAction* actOpen = nullptr;
    QAction* actClose = nullptr;
    QAction* actQuit = nullptr;
    QAction* actAbout = nullptr;
    QAction* actEditCredentials = nullptr;
    QAction* actEditPlayers = nullptr;
    QMenu*   menuFile = nullptr;
    QMenu*   menuDatabase = nullptr;
    QMenu*   menuHelp = nullptr;

    private slots:
    void onOpen();
    void onClose();
    void onQuit();
    void onAbout();
    void onEditCredentials();
    void onEditPlayers();
    void refreshTable();
};