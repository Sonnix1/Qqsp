#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Quest Soft Player");
    a.setApplicationVersion("5.7.0");

    MainWindow w;
    w.show();

    return a.exec();
}
