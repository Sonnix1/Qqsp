#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Qqsp");
    a.setOrganizationName("Qqsp");
    a.setApplicationVersion("0.2");

    MainWindow w;
    w.show();

    return a.exec();
}
