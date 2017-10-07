#include "qspwebengineurlschemehandler.h"

#include <QBuffer>
#include <QUrl>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFile>
#include <QDebug>

#include "comtools.h"

QspWebEngineUrlSchemeHandler::QspWebEngineUrlSchemeHandler(QObject *parent) : QWebEngineUrlSchemeHandler(parent)
{

}

void QspWebEngineUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request)
{
    const QUrl url = request->requestUrl();
    QString url_str = QByteArray::fromPercentEncoding(url.toString().toUtf8());
    qDebug() << "requestStarted: " << url;
    QBuffer *buffer = new QBuffer;
    connect(request, SIGNAL(destroyed()), buffer, SLOT(deleteLater()));

    QString replystr;
    replystr.append("<html>\n<head>\n<style>\nbody {\n");
    if(m_backColor.isValid())
        replystr.append(QString("background-color: %1;\n").arg(m_backColor.name()));
    if(m_fontColor.isValid())
        replystr.append(QString("color: %1;\n").arg(m_fontColor.name()));
    replystr.append(QString("font-family: %1;\n").arg(m_font.family()));
    if(!m_bmpBg.isEmpty())
        replystr.append(QString("background: url(%1) no-repeat center center fixed;\nbackground-size: cover;\n").arg(m_bmpBg));
    if(m_linkColor.isValid())
        replystr.append(QString("}\na:link {\ncolor: %1;\n").arg(m_linkColor.name()));
    replystr.append("}\n</style></head>\n<body>\n");
    replystr.append(m_text);
    replystr.append("</body>\n</html>");
    qDebug() << replystr;

    buffer->open(QIODevice::WriteOnly);
    if(url_str.compare("qsp:" , Qt::CaseInsensitive) == 0 || url_str.compare("qsp:/" , Qt::CaseInsensitive) == 0)
    {
        buffer->write(replystr.toUtf8());
        buffer->close();
        request->reply("text/html", buffer);
    }
    else
    {
        QString path = QSPTools::GetCaseInsensitiveFilePath(m_path, url_str.mid(5));
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(m_path + path);
        qDebug() << "Mime type:" << type.name();
        QFile file( m_path + path );
        file.open(QIODevice::ReadOnly);
        buffer->write(file.readAll());
        buffer->close();
        file.close();
        request->reply(type.name().toUtf8(), buffer);
    }
}

void QspWebEngineUrlSchemeHandler::SetPlainText(const QString &text)
{
    m_text= text;
}

void QspWebEngineUrlSchemeHandler::SetHtml(const QString &text)
{
    m_text =text;
}

void QspWebEngineUrlSchemeHandler::SetGamePath(const QString &path)
{
    m_path = path;
}

void QspWebEngineUrlSchemeHandler::SetLinkColor(const QColor &color)
{
    m_linkColor = color;
}

void QspWebEngineUrlSchemeHandler::SetBackgroundColor(const QColor &color)
{
    m_backColor = color;
}

void QspWebEngineUrlSchemeHandler::SetForegroundColor(const QColor &color)
{
    m_fontColor = color;
}

void QspWebEngineUrlSchemeHandler::SetBackgroundImage(const QString &path)
{
    m_bmpBg = path;
}

void QspWebEngineUrlSchemeHandler::SetTextFont(const QFont &new_font)
{
    m_font = new_font;
}