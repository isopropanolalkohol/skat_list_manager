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

#include "central_view.h"
#include "data_service.h"
#include "entries_table_model.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
    explicit MainWindow(DataService& ds, QWidget* parent = nullptr);

    private:
    DataService& service_;

    EntriesTableModel* model_ = nullptr;
    CentralView* central_ = nullptr;
    bool saved_;
    void createActions();
    void buildMenus();
    void buildStatusBar();
    void connectActions();

    void markDirty();
    void markClean();
    [[nodiscard]] bool maybeSave() const;

    QAction* actOpen = nullptr;
    QAction* actClose = nullptr;
    QAction* actQuit = nullptr;
    QAction* actAbout = nullptr;
    QAction* actSave = nullptr;
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
    void onSave();
    void refreshTable();
};
