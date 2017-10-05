#include "qspwebengineurlschemehandler.h"
#include <QBuffer>
#include <QDebug>

QspWebEngineUrlSchemeHandler::QspWebEngineUrlSchemeHandler(QObject *parent) : QWebEngineUrlSchemeHandler(parent)
{

}

void QspWebEngineUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request)
{
    const QUrl url = request->requestUrl();
    qDebug() << "requestStarted: " << url;
    QBuffer *buffer = new QBuffer;
    connect(request, SIGNAL(destroyed()), buffer, SLOT(deleteLater()));

    buffer->open(QIODevice::WriteOnly);
    buffer->write("<html><body>Hello world!</body></html>");
    buffer->close();

    request->reply("text/html", buffer);
}
