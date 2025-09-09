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

    void closeEvent(QCloseEvent* event) override;

    private:
    DataService& ds_;

    EntriesTableModel* model_ = nullptr;
    CentralView* central_ = nullptr;
    bool dirty_;
    void createActions();
    void buildMenus();
    void buildStatusBar();
    void connectActions();

    void markDirty();
    void markClean();
    [[nodiscard]] bool maybeSave();

    QAction* actOpen = nullptr;
    QAction* actClose = nullptr;
    QAction* actQuit = nullptr;
    QAction* actAbout = nullptr;
    QAction* actSave = nullptr;
    QAction* actEditCredentials = nullptr;
    QAction* actEditPlayers = nullptr;
    QAction* actDebugDirty = nullptr;
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
