//
// Created by joshua on 28.08.25.
//

#pragma once

#include <QObject>
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
    explicit MainWindow(QWidget* parent = nullptr);
};