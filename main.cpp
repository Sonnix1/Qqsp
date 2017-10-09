#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QString>
#include <QLocale>
#include <QCommandLineParser>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Qqsp");
    a.setOrganizationName("Qqsp");
    a.setApplicationVersion("1.1");

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

    QCommandLineParser parser;
    parser.setApplicationDescription("Qqsp");
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "Game file to open."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);

    MainWindow w;

    if(parser.positionalArguments().size() != 0)
    {
        QFileInfo file(parser.positionalArguments().at(0));
        w.OpenGameFile(file.filePath());
    }

    w.show();

    return a.exec();
}
