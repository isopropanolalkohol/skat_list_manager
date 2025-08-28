//
// Created by joshua on 28.08.25.
//
#include "main_window.h"
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("MyApp");
    resize(1100, 700);

    auto* central = new QWidget(this);
    central->setObjectName("central");
    setCentralWidget(central);
}
