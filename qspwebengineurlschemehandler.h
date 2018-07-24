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
    void SetHead(const QString &head);
    void SetFontType(const int fontType);
    void SetSizeType(const int sizeType);
    void SetCustomCSS(bool customCSS);

private:
    QString  m_text;
    QString m_path;
    QString m_bmpBg;
    QString m_head;
    QColor m_linkColor;
    QColor m_backColor;
    QColor m_fontColor;
    QFont m_font;
    bool m_isUseHtml = true;
    int m_fontType = 0;
    int m_sizeType = 0;
    bool m_isCustomCSS = false;
};

#endif // QSPWEBENGINEURLSCHEMEHANDLER_H
