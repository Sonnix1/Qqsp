#ifndef QSPWEBENGINEURLSCHEMEHANDLER_H
#define QSPWEBENGINEURLSCHEMEHANDLER_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QFont>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

class QspWebEngineUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    explicit QspWebEngineUrlSchemeHandler(QObject *parent = 0);
    void requestStarted(QWebEngineUrlRequestJob *request);
    void SetPlainText(const QString& text);
    void SetHtml(const QString& text);
    void SetGamePath(const QString &path);
    void SetLinkColor(const QColor &color);
    void SetBackgroundColor(const QColor& color);
    void SetForegroundColor(const QColor& color);
    void SetBackgroundImage(const QString &path);
    void SetTextFont(const QFont& new_font);

private:
    QString  m_text;
    QString m_path;
    QString m_bmpBg;
    QColor m_linkColor;
    QColor m_backColor;
    QColor m_fontColor;
    QFont m_font;
    bool m_isUseHtml = true;
};

#endif // QSPWEBENGINEURLSCHEMEHANDLER_H
