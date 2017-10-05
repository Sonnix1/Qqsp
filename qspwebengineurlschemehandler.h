#ifndef QSPWEBENGINEURLSCHEMEHANDLER_H
#define QSPWEBENGINEURLSCHEMEHANDLER_H

#include <QObject>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

class QspWebEngineUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    explicit QspWebEngineUrlSchemeHandler(QObject *parent = 0);
    void requestStarted(QWebEngineUrlRequestJob *request);

private:

};

#endif // QSPWEBENGINEURLSCHEMEHANDLER_H
