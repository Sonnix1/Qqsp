#include "qspwebbox.h"

#include <QFileInfo>
#include <QPalette>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QWebEngineProfile>

#include "comtools.h"

#include "qspwebengineurlrequestinterceptor.h"
#include "qspwebengineurlschemehandler.h"
#include "qspexecwebengineurlschemehandler.h"

QspWebBox::QspWebBox(QWidget *parent) : QWebEngineView(parent)
{
    setFocusPolicy(Qt::NoFocus);
    //setFrameStyle(QFrame::NoFrame);
    //setFrameShadow(QFrame::Plain);
    setContextMenuPolicy( Qt::NoContextMenu );
    setContentsMargins(0,0,0,0);
    m_isUseHtml = false;
    showPlainText = false;
    m_font = font();
    //setOpenLinks(false);
    QWebEngineProfile *profile = new QWebEngineProfile(this);
    //QspWebEngineUrlRequestInterceptor *qwuri = new QspWebEngineUrlRequestInterceptor(this);
    //profile->setRequestInterceptor(qwuri);
    qweush = new QspWebEngineUrlSchemeHandler(this);
    profile->installUrlSchemeHandler(QByteArray("qsp"),qweush);
    QspExecWebEngineUrlSchemeHandler *qeweush = new QspExecWebEngineUrlSchemeHandler(this);
    profile->installUrlSchemeHandler(QByteArray("exec"),qeweush);
    QWebEnginePage * page = new QWebEnginePage(profile, this);
    setPage(page);
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
    text = QSPTools::HtmlizeWhitespaces(m_isUseHtml ? str : QSPTools::ProceedAsPlain(str));
    if(showPlainText)
        qweush->SetPlainText(text);
    else
        qweush->SetHtml(text);
    page()->load(QUrl("qsp:/"));
    //if (isScroll) verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QspWebBox::LoadBackImage(const QString& fileName)
{
    qweush->SetBackgroundImage(fileName);
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
        qweush->SetTextFont(new_font);
    }
}

bool QspWebBox::SetLinkColor(const QColor &color)
{
    if(m_linkColor != color)
    {
        m_linkColor = color;
        qweush->SetLinkColor(color);
        RefreshUI();
        return true;
    }
    return false;
}

void QspWebBox::SetGamePath(const QString &path)
{
    m_path = path;
    qweush->SetGamePath(path);
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
        qweush->SetBackgroundColor(color);
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
        qweush->SetForegroundColor(color);
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
