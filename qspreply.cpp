#include "qspreply.h"

#include <QTimer>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFile>

#include "comtools.h"

QspReply::QspReply(const QUrl &url, const QString &_text, bool _isUseHtml, const QString &_path, const QColor &_linkColor, const QColor &_backColor, const QColor &_fontColor, const QString &_bmpBg, const QFont &_font)
{
    offset = 0;
    m_text = _text;
    m_path = _path;
    m_bmpBg = _bmpBg;
    m_linkColor = _linkColor;
    m_backColor = _backColor;
    m_fontColor = _fontColor;
    m_font = _font;
    m_isUseHtml = _isUseHtml;

    setUrl(url);

    QString url_str = QByteArray::fromPercentEncoding(url.toString().toUtf8());

    if(url_str.compare("qsp:" , Qt::CaseInsensitive) == 0 || url_str.compare("qsp:/" , Qt::CaseInsensitive) == 0)
    {
        QString replystr;
        replystr.append("<html>\n<head>\n<meta charset=\"UTF-8\">\n<style>\nbody {\n");
        if(m_backColor.isValid())
            replystr.append(QString("background-color: %1;\n").arg(m_backColor.name()));
        if(m_fontColor.isValid())
            replystr.append(QString("color: %1;\n").arg(m_fontColor.name()));
        replystr.append(QString("font-family: %1;\n").arg(m_font.family()));
        replystr.append(QString("font-size: %1pt;\n").arg(m_font.pointSize()));
        if(!m_bmpBg.isEmpty())
            replystr.append(QString("background: url(%1) no-repeat center center fixed;\nbackground-size: cover;\n").arg(m_bmpBg));
        if(m_linkColor.isValid())
            replystr.append(QString("}\na:link {\ncolor: %1;\n").arg(m_linkColor.name()));
        replystr.append("}\n</style></head>\n<body>\n");
        replystr.append(m_text);
        replystr.append("</body>\n</html>");

        open(ReadOnly | Unbuffered);
        if(m_isUseHtml)
        {
            content = replystr.toUtf8();
            setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html"));
        }
        else
        {
            setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain; charset=utf-8"));
            content = m_text.toUtf8();
        }
    }
    else
    {
        open(ReadOnly | Unbuffered);
        QString path = QSPTools::GetCaseInsensitiveFilePath(m_path, url_str.mid(5));
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(m_path + path);
        QFile file( m_path + path );
        if(file.open(QIODevice::ReadOnly))
        {
            content = file.readAll();
            setHeader(QNetworkRequest::ContentTypeHeader, QVariant(type.name().toUtf8()));
        }
        file.close();
    }
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));
    QTimer::singleShot(0, this, SLOT(readyRead()));
    QTimer::singleShot(0, this, SLOT(finished()));
    //emit readyRead();
    //emit finished();
}

void QspReply::abort()
{

}

qint64 QspReply::bytesAvailable() const
{
    return content.size() - offset + QIODevice::bytesAvailable();
}

bool QspReply::isSequential() const
{
    return true;
}

qint64 QspReply::readData(char *data, qint64 maxSize)
{
    if (offset < content.size()) {
        qint64 number = qMin(maxSize, content.size() - offset);
        memcpy(data, content.constData() + offset, number);
        offset += number;
        return number;
    } else
        return -1;
}
