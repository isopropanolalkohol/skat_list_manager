#include <QApplication>
#include "main_window.h"
#include "database_api.h"
#include "data_service.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":icons/desktop.png"));
    app.setApplicationName("Skat List Manager");
    app.setOrganizationName("GeisslerInc");
    DatabaseAPI api;
    DataService ds{api};
    MainWindow w{ds};
    w.show();
    return app.exec();
}
