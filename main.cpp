#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QString>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Qqsp");
    a.setOrganizationName("Qqsp");
    a.setApplicationVersion("0.4");

    QObject::tr("__LANGNAME__");
    QObject::tr("__LANGID__");

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName());
    QTranslator qtTranslator;
    QString langid = settings.value("application/language", QLocale::system().name()).toString();
    if(qtTranslator.load(QApplication::applicationName() + "." + langid, QApplication::applicationDirPath()))
        a.installTranslator(&qtTranslator);
    else
        if(qtTranslator.load(QApplication::applicationName() + "." + langid, ":/translations/"))
            a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
