#include "qspwebengineurlschemehandler.h"

#include <QBuffer>
#include <QUrl>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFile>

#include "comtools.h"

QspWebEngineUrlSchemeHandler::QspWebEngineUrlSchemeHandler(QObject *parent) : QWebEngineUrlSchemeHandler(parent)
{

}

void QspWebEngineUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request)
{
    const QUrl url = request->requestUrl();
    QString url_str = url.toString();
    QBuffer *buffer = new QBuffer;
    connect(request, SIGNAL(destroyed()), buffer, SLOT(deleteLater()));

    buffer->open(QIODevice::WriteOnly);
    if(url_str.compare("qsp:" , Qt::CaseInsensitive) == 0 || url_str.compare("qsp:/" , Qt::CaseInsensitive) == 0)
    {
        if(m_isUseHtml)
        {
            QString replystr;
            replystr.append("<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n<style>\nbody {\n");
            if(m_backColor.isValid())
                replystr.append(QString("background-color: %1;\n").arg(m_backColor.name()));
            if(m_fontColor.isValid())
                replystr.append(QString("color: %1;\n").arg(m_fontColor.name()));
            if(m_fontType == 1) //set in qsp
                replystr.append(QString("font-family: %1;\n").arg(m_font.family()));
            if(m_fontType == 2) //set by user
                replystr.append(QString("font-family: %1 !important;\n").arg(m_font.family()));
            if(m_sizeType == 1) //set in qsp
                replystr.append(QString("font-size: %1pt;\n").arg(m_font.pointSize()));
            if(m_sizeType == 2) //set by user
                replystr.append(QString("font-size: %1pt !important;\n").arg(m_font.pointSize()));
            if(!m_bmpBg.isEmpty())
                replystr.append(QString("background: url(%1) no-repeat center center fixed;\nbackground-size: cover;\n").arg(m_bmpBg));
            if(m_linkColor.isValid())
                replystr.append(QString("}\na:link {\ncolor: %1;\n").arg(m_linkColor.name()));
            replystr.append("}\n</style>\n");
            if(m_isCustomCSS)
                replystr.append("<link rel=\"stylesheet\" type=\"text/css\" href=\"custom.css\">\n");
            replystr.append("<script src=\"qrc:///qtwebchannel/qwebchannel.js\"></script>\n");
            replystr.append("<script type=\"text/javascript\"> \nvar qsp; \ndocument.addEventListener(\"DOMContentLoaded\", function () { \nnew QWebChannel(qt.webChannelTransport, function(channel) { \nqsp = channel.objects.qsp; \n}); \n});\n</script>\n");
            replystr.append(m_head);
            replystr.append("</head>\n<body>\n");
            replystr.append(m_text);
            replystr.append("</body>\n</html>");

            buffer->write(replystr.toUtf8());
            buffer->close();
            request->reply("text/html", buffer);
        }
        else
        {
            buffer->write(m_text.toUtf8());
            buffer->close();
            request->reply("text/plain; charset=utf-8", buffer);
        }
    }
    else
    {
        QString tmpurl = url_str.mid(5);
        if(tmpurl.contains('#') && !tmpurl.isEmpty())
        {
            tmpurl = tmpurl.split('#').at(0);
        }
        tmpurl = QByteArray::fromPercentEncoding(tmpurl.toUtf8());
        QString path = QSPTools::GetCaseInsensitiveFilePath(m_path, tmpurl);
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(m_path + path);
        QFile file( m_path + path );
        if(file.open(QIODevice::ReadOnly))
        {
            buffer->write(file.readAll());
        }
        buffer->close();
        file.close();
        request->reply(type.name().toUtf8(), buffer);
    }
}

void QspWebEngineUrlSchemeHandler::SetPlainText(const QString &text)
{
    m_isUseHtml = false;
    m_text= text;
}

void QspWebEngineUrlSchemeHandler::SetHtml(const QString &text)
{
    m_isUseHtml = true;
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

void QspWebEngineUrlSchemeHandler::SetHead(const QString &head)
{
    m_head = head;
}

void QspWebEngineUrlSchemeHandler::SetFontType(const int fontType)
{
    m_fontType = fontType;
}

void QspWebEngineUrlSchemeHandler::SetSizeType(const int sizeType)
{
    m_sizeType = sizeType;
}

void QspWebEngineUrlSchemeHandler::SetCustomCSS(bool customCSS)
{
    m_isCustomCSS = customCSS;
}
