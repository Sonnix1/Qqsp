#ifndef QSPTEXTBOX_H
#define QSPTEXTBOX_H

#include <QWidget>
#include <QTextBrowser>
#include <QString>
#include <QFont>
#include <QColor>
#include <QPixmap>
#include <QVariant>
#include <QUrl>
#include <QMovie>
#include <QMap>

#include "videoframeprocessor.h"

struct animation_gif
{
    int x;
    int y;
    int w;
    int h;
    QMovie *movie;
};

struct animation_video
{
    int x;
    int y;
    int w;
    int h;
    VideoFrameProcessor *frameProcessor;
};

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
    void SetTextFont(const QFont& new_font);
    QFont GetTextFont() const { return m_font; }
    QString GetText() const { return m_text; }
    bool SetLinkColor(const QColor &color);
    QColor GetLinkColor() { return m_linkColor; }
    void SetGamePath(const QString& path);
    void SetBackgroundImage(const QPixmap& bmpBg);
    QColor GetBackgroundColor();
    QColor GetForegroundColor(); //text color
    bool SetBackgroundColor(const QColor& color);
    bool SetForegroundColor(const QColor& color);
    void SetShowPlainText(bool isPlain);

private:
    // Internal methods
    void CalcImageSize();
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    QVariant loadResource(int type, const QUrl &name);
    void wheelEvent(QWheelEvent *e);

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
    QColor m_backColor;
    QColor m_fontColor;
    bool showPlainText;
    QMap<QString, animation_gif> animations_gif;
    QMap<QString, animation_video> animations_video;

private slots:
    void repaintAnimation();
    void resizeAnimations();
};

#endif // QSPTEXTBOX_H
