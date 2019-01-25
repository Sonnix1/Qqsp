#include "qspwebbox.h"

#include <QFileInfo>
#include <QPalette>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QWebEngineSettings>
//#include <QTimer>
#include <QEventLoop>

#include "comtools.h"

//#include "qspwebengineurlrequestinterceptor.h"

QspWebBox::QspWebBox(QWidget *parent) : QWebEngineView(parent)
{
    m_isQuit = false;
    settings()->setDefaultTextEncoding("utf-8");
    setFocusPolicy(Qt::NoFocus);
    settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
    settings()->setUnknownUrlSchemePolicy(QWebEngineSettings::AllowAllUnknownUrlSchemes);
    //setFrameStyle(QFrame::NoFrame);
    //setFrameShadow(QFrame::Plain);
    setContextMenuPolicy( Qt::NoContextMenu );
    setContentsMargins(0,0,0,0);
    m_isUseHtml = false;
    showPlainText = false;
    m_videoFix = true;
    m_font = font();
    //setOpenLinks(false);
    //QspWebEngineUrlRequestInterceptor *qwuri = new QspWebEngineUrlRequestInterceptor(this);
    //profile->setRequestInterceptor(qwuri);
    profile.installUrlSchemeHandler(QByteArray("qsp"), &qweush);
    profile.installUrlSchemeHandler(QByteArray("exec"), &qeweush);
    page()->triggerAction(QWebEnginePage::Stop);
    page()->deleteLater();
    QWebEnginePage * newpage = new QWebEnginePage(&profile, this);
    QWebChannel *channel = new QWebChannel(newpage);
    channel->registerObject(QStringLiteral("qsp"), &qspJS);
    newpage->setWebChannel(channel);
    newpage->settings()->setDefaultTextEncoding("utf-8");
    newpage->settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
    newpage->settings()->setUnknownUrlSchemePolicy(QWebEngineSettings::AllowAllUnknownUrlSchemes);
    page()->deleteLater();
    setPage(newpage);
    connect(&qeweush, SIGNAL(qspLinkClicked(QUrl)), this, SLOT(OnQspLinkClicked(QUrl)));
    QEventLoop loop;
    connect(page(), SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    page()->load(QUrl("qsp:/"));
    loop.exec();
}

QspWebBox::~QspWebBox()
{

}

void QspWebBox::SetIsHtml(bool isHtml)
{
    if (m_isUseHtml != isHtml)
    {
        m_isUseHtml = isHtml;
        RefreshUI();
    }
}

void QspWebBox::RefreshUI(bool isScroll)
{
    if(m_isQuit)
        return;
    QString color(QSPTools::GetHexColor(GetForegroundColor()));
    QString str = m_text;
    QString text;
//    if(m_isUseHtml)
//    {
//        if(str.endsWith("\r"))
//            str.chop(1);
//        if(str.endsWith("\n"))
//            str.chop(1);
//        str = str.replace("\r", "").replace("\n", "<br>").replace("<video ", "<img ", Qt::CaseInsensitive);
//        text = str.replace("</center><br>", "</center>", Qt::CaseInsensitive).replace("</table><br>", "</table>", Qt::CaseInsensitive);
//    }
//    else
//    {
//        text = str;
//    }

    if(m_videoFix)
    {
        int copypos = 0;
        int startIndex = str.indexOf("<video", 0, Qt::CaseInsensitive);
        while (startIndex >= 0)
        {
            int endIndex = str.indexOf(">", startIndex, Qt::CaseInsensitive);
            if(endIndex < 0)
                break;
            endIndex = endIndex + 1;
            text.append(str.mid(copypos, startIndex + 6 - copypos));
            if(!str.mid(startIndex, endIndex - startIndex).contains("autoplay", Qt::CaseInsensitive))
                text.append(" autoplay");
            if(!str.mid(startIndex, endIndex - startIndex).contains("loop", Qt::CaseInsensitive))
                text.append(" loop");
            text.append(str.mid(startIndex + 6, endIndex - startIndex - 6));
            copypos = endIndex;
            int cloaseTegPos = str.indexOf("</video>", 0, Qt::CaseInsensitive);
            if(cloaseTegPos == -1)
                text.append("</video>");
            else
            {
                int nextV = str.indexOf("<video", endIndex, Qt::CaseInsensitive);
                if(nextV == -1)
                    text.append("</video>");
                else
                {
                    if(cloaseTegPos > nextV)
                        text.append("</video>");
                }
            }
            startIndex = str.indexOf("<video", endIndex, Qt::CaseInsensitive);
        }
        text.append(str.mid(copypos));
    }
    else
        text = str;

    text = QSPTools::HtmlizeWhitespaces(m_isUseHtml ? text : QSPTools::ProceedAsPlain(text));
    if(showPlainText)
        qweush.SetPlainText(text);
    else
        qweush.SetHtml(text);

    QString url_str = QByteArray::fromPercentEncoding(url().toString().toUtf8());
    if(url_str.compare("qsp:" , Qt::CaseInsensitive) != 0 && url_str.compare("qsp:/" , Qt::CaseInsensitive) != 0)
    {
        page()->triggerAction(QWebEnginePage::Stop);
        page()->deleteLater();
        QWebEnginePage * newpage = new QWebEnginePage(&profile, this);
        QWebChannel *channel = new QWebChannel(newpage);
        channel->registerObject(QStringLiteral("qsp"), &qspJS);
        newpage->setWebChannel(channel);
        newpage->settings()->setDefaultTextEncoding("utf-8");
        newpage->settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
        newpage->settings()->setUnknownUrlSchemePolicy(QWebEngineSettings::AllowAllUnknownUrlSchemes);
        setPage(newpage);
        QEventLoop loop;
        connect(page(), SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
        page()->load(QUrl("qsp:/"));
        loop.exec();
    }
    page()->triggerAction(QWebEnginePage::ReloadAndBypassCache);

    //QTimer wtimer;
    //wtimer.setSingleShot(true);
    //QEventLoop loop;
    //connect(this,  SIGNAL(loadFinished(bool)), &loop, SLOT(quit()) );
    //connect(&wtimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    //wtimer.start(400);
    //loop.exec();
    //if(!wtimer.isActive())
    //{
    //    qDebug() << "timeout";
    //}
    //if (isScroll) verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QspWebBox::LoadBackImage(const QString& fileName)
{
    qweush.SetBackgroundImage(fileName);
    QFileInfo file(m_path + fileName);
    QString path(file.absoluteFilePath());
}

void QspWebBox::SetText(const QString& text, bool isScroll)
{
    if (m_text != text)
    {
        if (isScroll)
        {
            if (m_text.isEmpty() || !text.startsWith(m_text))
                isScroll = false;
        }
        m_text = text;
        RefreshUI(isScroll);
    }
}

void QspWebBox::SetTextFont(const QFont& new_font)
{
    if (m_font != new_font)
    {
        m_font = new_font;
        qweush.SetTextFont(new_font);
    }
}

bool QspWebBox::SetLinkColor(const QColor &color)
{
    if(m_linkColor != color)
    {
        m_linkColor = color;
        qweush.SetLinkColor(color);
        RefreshUI();
        return true;
    }
    return false;
}

void QspWebBox::SetGamePath(const QString &path)
{
    m_path = path;
    qweush.SetGamePath(path);
}

//Returns the background color of the window.
QColor QspWebBox::GetBackgroundColor()
{
    return m_backColor;
}

//The meaning of foreground colour varies according to the window class; it may be the text colour or other colour, or it may not be used at all. Additionally, not all native controls support changing their foreground colour so this method may change their colour only partially or even not at all.
QColor QspWebBox::GetForegroundColor()
{
    return m_fontColor;
}

//Returns true if the color was really changed, false if it was already set to this color and nothing was done.
bool QspWebBox::SetBackgroundColor(const QColor &color)
{
    if(m_backColor != color)
    {
        m_backColor = color;
        qweush.SetBackgroundColor(color);
        RefreshUI();
        return true;
    }
    return false;
}

bool QspWebBox::SetForegroundColor(const QColor &color)
{
    if(m_fontColor != color)
    {
        m_fontColor = color;
        qweush.SetForegroundColor(color);
        RefreshUI();
        return true;
    }
    return false;
}

void QspWebBox::SetShowPlainText(bool isPlain)
{
    showPlainText = isPlain;
    RefreshUI();
}

void QspWebBox::SetVideoFix(bool isFix)
{
    m_videoFix = isFix;
}

void QspWebBox::SetHead(const QString &head)
{
    if(m_head != head)
    {
        m_head = head;
        qweush.SetHead(head);
        RefreshUI();
    }
}

void QspWebBox::SetFontType(const int fontType)
{
    if(m_fontType != fontType)
    {
        m_fontType = fontType;
        qweush.SetFontType(fontType);
        RefreshUI();
    }
}

void QspWebBox::SetSizeType(const int sizeType)
{
    if(m_sizeType != sizeType)
    {
        m_sizeType = sizeType;
        qweush.SetSizeType(sizeType);
        RefreshUI();
    }
}

void QspWebBox::SetCustomCSS(bool customCSS)
{
    qweush.SetCustomCSS(customCSS);
}

void QspWebBox::Quit()
{
    m_isQuit = true;
    page()->triggerAction(QWebEnginePage::Stop);
    page()->deleteLater();
    QWebEnginePage * newpage = new QWebEnginePage(this);
    setPage(newpage);
    QEventLoop loop;
    connect(page(), SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    page()->load(QUrl("about:blank"));
    loop.exec();
}

void QspWebBox::OnQspLinkClicked(QUrl url)
{
    emit qspLinkClicked(url);
}
