#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QColor>
#include <qsp_default.h>

class QSPTools
{
public:
    static QString GetHexColor(const QColor color);
    static QString HtmlizeWhitespaces(const QString& str);
    static QString ProceedAsPlain(const QString& str);
    static QString GetAppPath();
    static QString qspStrToQt(const QSP_CHAR *str);
};

#endif
