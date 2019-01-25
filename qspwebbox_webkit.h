#ifndef QSPWEBBOX_H
#define QSPWEBBOX_H

#include <QWidget>
#include <QWebView>
#include <QString>
#include <QFont>
#include <QColor>
#include <QVariant>
#include <QUrl>

#include "qspnetworkaccessmanager.h"

namespace Ui {
class QspWebBox;
}

class QspWebBox : public QWebView
{
    Q_OBJECT

public:
    explicit QspWebBox(QWidget *parent = 0);
    ~QspWebBox();

    // Methods
    void RefreshUI(bool isScroll = false);
    void LoadBackImage(const QString& fileName);

    // Accessors
    void SetIsHtml(bool isHtml);
    void SetText(const QString& text, bool isScroll = false);
    void SetTextFont(const QFont& new_font);
    QFont GetTextFont() const { return m_font; }
    QString GetText() const { return m_text; }
    bool SetLinkColor(const QColor &color);
    QColor GetLinkColor() { return m_linkColor; }
    void SetGamePath(const QString& path);
    QColor GetBackgroundColor();
    QColor GetForegroundColor(); //text color
    bool SetBackgroundColor(const QColor& color);
    bool SetForegroundColor(const QColor& color);
    void SetShowPlainText(bool isPlain);
    void SetVideoFix(bool isFix);
    //TODO:
    void SetHead(const QString &head) {};
    void SetFontType(const int fontType) {};
    void SetSizeType(const int sizeType) {};
    void SetCustomCSS(bool customCSS) {};
    void Quit();

private:
    // Fields
    bool m_isUseHtml;
    QString m_outFormat;
    QString m_path;
    QString m_imagePath;
    QFont m_font;
    QString m_text;
    int m_posX;
    int m_posY;
    QColor m_linkColor;
    QColor m_backColor;
    QColor m_fontColor;
    bool showPlainText;
    bool m_videoFix;
    QspNetworkAccessManager *qspManager;
};

#endif // QSPWEBBOX_H
