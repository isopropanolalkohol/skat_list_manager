#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":icons/desktop.png"));
    app.setApplicationName("Skat List Manager");
    app.setOrganizationName("GeisslerInc");

    MainWindow w;
    w.show();
    return app.exec();
}
