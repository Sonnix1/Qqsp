#ifndef QSPTEXTBOX_H
#define QSPTEXTBOX_H

#include <QWidget>
#include <QTextBrowser>
#include <QString>
#include <QFont>
#include <QColor>
#include <QPixmap>

namespace Ui {
class QspTextBox;
}

class QspTextBox : public QTextBrowser
{
    Q_OBJECT

public:
    explicit QspTextBox(QWidget *parent = 0);
    ~QspTextBox();

    // Methods
    void RefreshUI(bool isScroll = false);
    void LoadBackImage(const QString& fileName);

    // Accessors
    void SetIsHtml(bool isHtml);
    void SetText(const QString& text, bool isScroll = false);
    void SetTextFont(const QFont& font);
    QFont GetTextFont() const { return m_font; }
    QString GetText() const { return m_text; }
    void SetLinkColor(const QColor& clr);
    QColor GetLinkColor() { return m_linkColor; }
    void SetGamePath(const QString& path);
    void SetBackgroundImage(const QPixmap& bmpBg);
    QColor GetBackgroundColour();
    QColor GetForegroundColour();
    bool SetBackgroundColour(const QColor& colour);
    bool SetForegroundColour(const QColor& colour);
    void SetShowPlainText(bool isPlain);

private:
    // Internal methods
    void CalcImageSize();

    // Fields
    bool m_isUseHtml;
    QString m_outFormat;
    QString m_path;
    QString m_imagePath;
    QFont m_font;
    QString m_text;
    QPixmap m_bmpBg;
    QPixmap m_bmpRealBg;
    int m_posX;
    int m_posY;
    QColor m_linkColor;
    bool showPlainText;
};

#endif // QSPTEXTBOX_H
