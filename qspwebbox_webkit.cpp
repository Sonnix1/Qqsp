#include "qspwebbox_webkit.h"

#include <QFileInfo>
#include <QPalette>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QWebSettings>
#include <QEventLoop>

#include "comtools.h"

QspWebBox::QspWebBox(QWidget *parent) : QWebView(parent)
{
    settings()->setDefaultTextEncoding("utf-8");
    setFocusPolicy(Qt::NoFocus);
    //setFrameStyle(QFrame::NoFrame);
    //setFrameShadow(QFrame::Plain);
    setContextMenuPolicy( Qt::NoContextMenu );
    setContentsMargins(0,0,0,0);
    m_isUseHtml = false;
    showPlainText = false;
    m_videoFix = true;
    m_font = font();
    //setOpenLinks(false);
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    QNetworkAccessManager *oldManager = page()->networkAccessManager();
    qspManager = new QspNetworkAccessManager(oldManager, this);
    page()->setNetworkAccessManager(qspManager);
    page()->setForwardUnsupportedContent(true);
    load(QUrl("qsp:/"));
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
        qspManager->SetPlainText(text);
    else
        qspManager->SetHtml(text);
    QString url_str = QByteArray::fromPercentEncoding(url().toString().toUtf8());
    if(url_str.compare("qsp:" , Qt::CaseInsensitive) != 0 && url_str.compare("qsp:/" , Qt::CaseInsensitive) != 0)
    {
        QEventLoop loop;
        connect(this, SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
        load(QUrl("qsp:/"));
        loop.exec();
    }
    triggerPageAction(QWebPage::ReloadAndBypassCache);
}

void QspWebBox::LoadBackImage(const QString& fileName)
{
    qspManager->SetBackgroundImage(fileName);
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
        qspManager->SetTextFont(new_font);
    }
}

bool QspWebBox::SetLinkColor(const QColor &color)
{
    if(m_linkColor != color)
    {
        m_linkColor = color;
        qspManager->SetLinkColor(color);
        RefreshUI();
        return true;
    }
    return false;
}

void QspWebBox::SetGamePath(const QString &path)
{
    m_path = path;
    qspManager->SetGamePath(path);
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
        qspManager->SetBackgroundColor(color);
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
        qspManager->SetForegroundColor(color);
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
