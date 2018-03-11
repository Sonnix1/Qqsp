#ifndef QSPREPLY_H
#define QSPREPLY_H

#include <QNetworkReply>
#include <QString>
#include <QColor>
#include <QFont>

class QspReply : public QNetworkReply
{
    Q_OBJECT

public:
    QspReply(const QUrl &url, const QString  &_text, bool _isUseHtml, const QString &_path, const QColor &_linkColor, const QColor &_backColor, const QColor &_fontColor, const QString &_bmpBg, const QFont &_font);
    void abort();
    qint64 bytesAvailable() const;
    bool isSequential() const;

protected:
    qint64 readData(char *data, qint64 maxSize);

    QByteArray content;
    qint64 offset;

    QString  m_text;
    QString m_path;
    QString m_bmpBg;
    QColor m_linkColor;
    QColor m_backColor;
    QColor m_fontColor;
    QFont m_font;
    bool m_isUseHtml = true;
};

#endif // QSPREPLY_H
