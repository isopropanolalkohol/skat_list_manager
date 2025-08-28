#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Skat List Manager");
    app.setOrganizationName("YourOrg");

    MainWindow w;
    w.show();
    return app.exec();
}
