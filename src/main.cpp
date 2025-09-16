#include <QApplication>
#include "main_window.h"
#include "database_api.h"
#include "data_service.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":icons/desktop.png"));
    QApplication::setApplicationName("Skat List Manager");
    QApplication::setOrganizationName("GeisslerInc");
    DatabaseAPI api;
    DataService ds{api};
    MainWindow w{ds};
    w.show();
    return QApplication::exec();

    // bla bli blub
}
