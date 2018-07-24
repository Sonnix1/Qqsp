#ifndef QSPTEXTBOX_H
#define QSPTEXTBOX_H

#include <QWidget>
#include <QTextBrowser>
#include <QString>
#include <QFont>
#include <QColor>
#include <QImage>
#include <QVariant>
#include <QUrl>
#include <QMovie>
#include <QLabel>
#include <QMap>
#include <QKeyEvent>
#include <QMutex>

#include "videolabel.h"

struct animation_gif
{
    QMovie *movie;
    QLabel *movieLabel;
};

struct animation_video
{
    VideoLabel *videoLabel;
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
    void SetDisableVideo(bool isDisableVideo) { disableVideo = isDisableVideo; }
    void keyPressEvent(QKeyEvent *event);
#ifndef _WEBBOX_COMMON
    void SetBackgroundImage(const QImage& bmpBg);
    void LoadBackImage(const QString& fileName);
    //TODO:
    void SetHead(const QString &head) {};
    void SetFontType(const int fontType) {};
    void SetSizeType(const int sizeType) {};
    void SetCustomCSS(bool customCSS) {};
#endif

private:
    // Internal methods
    void wheelEvent(QWheelEvent *e);
#ifndef _WEBBOX_COMMON
    void CalcImageSize();
    void paintEvent(QPaintEvent *e);
    QVariant loadResource(int type, const QUrl &name);
    void resizeEvent(QResizeEvent *e);
    void clearManualResources();
#endif

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
    bool disableVideo;
#ifndef _WEBBOX_COMMON
    QImage m_bmpBg;
    QImage m_bmpRealBg;
    QMap<QString, animation_gif> animations_gif;
    QMap<QString, animation_video> animations_video;
    QMutex mutex;
#endif

#ifndef _WEBBOX_COMMON
private slots:
    void repaintAnimation();
    void resizeAnimations();
#endif
};

#endif // QSPTEXTBOX_H
