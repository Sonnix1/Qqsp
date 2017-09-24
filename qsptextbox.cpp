#include "qsptextbox.h"

#include <QFileInfo>
#include <QPalette>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QDir>
#include <QDirIterator>

#include "comtools.h"

QspTextBox::QspTextBox(QWidget *parent) : QTextBrowser(parent)
{
    //SetBorders(5);
    setFocusPolicy(Qt::NoFocus);
    setFrameStyle(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setContentsMargins(0,0,0,0);
    m_isUseHtml = false;
    showPlainText = false;
    m_linkColor = palette().color(QPalette::Link);
    m_fontColor = palette().color(QPalette::Text);
    m_backColor = palette().color(QPalette::Window);
    m_font = font();
    setOpenLinks(false);
//	m_font = *wxNORMAL_FONT;
//	m_outFormat = wxString::Format(
//		wxT("<HTML><META HTTP-EQUIV = \"Content-Type\" CONTENT = \"text/html; charset=%s\">")
//		wxT("<BODY><FONT COLOR = #%%s>%%s</FONT></BODY></HTML>"),
//		wxFontMapper::GetEncodingName(wxLocale::GetSystemEncoding()).wx_str()
//	);
}

QspTextBox::~QspTextBox()
{

}

void QspTextBox::SetIsHtml(bool isHtml)
{
    if (m_isUseHtml != isHtml)
    {
        m_isUseHtml = isHtml;
        RefreshUI();
    }
}

void QspTextBox::RefreshUI(bool isScroll)
{
    QString color(QSPTools::GetHexColor(GetForegroundColor()));
    //QString str(QByteArray::fromPercentEncoding(m_text.replace("%", "%25").toUtf8()));
    //QString text(QSPTools::HtmlizeWhitespaces(m_isUseHtml ? str : QSPTools::ProceedAsPlain(str)));
    QString str = m_text;
    QString text;
    if(m_isUseHtml)
    {
        if(str.endsWith("\r"))
            str.chop(1);
        if(str.endsWith("\n"))
            str.chop(1);
        text = str.replace("\r", "").replace("\n", "<br>");
    }
    else
    {
        text = Qt::convertFromPlainText(str);
    }
    //TODO: set colour and font
    if(showPlainText)
        setPlainText(text);
    else
        setHtml(text);
    if (isScroll) verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void QspTextBox::LoadBackImage(const QString& fileName)
{
    QFileInfo file(fileName);
    QString path(file.absoluteFilePath());
    if (m_imagePath != path)
    {
        m_imagePath = path;
        if (file.exists() && file.isFile())
        {
            QPixmap image;
            if (image.load(path))
            {
                SetBackgroundImage(image);
                //Refresh();
                return;
            }
        }
        SetBackgroundImage(QPixmap());
        //Refresh();
    }
}

void QspTextBox::SetText(const QString& text, bool isScroll)
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

void QspTextBox::SetTextFont(const QFont& new_font)
{
    if (m_font != new_font)
    {
        m_font = new_font;
        setFont(new_font); //TODO: check which one of this to use
        setCurrentFont(new_font);
    }
}

bool QspTextBox::SetLinkColor(const QColor &color)
{
    //QPalette p = palette();
    //p.setBrush( QPalette::Link, clr);
    //setPalette( p );
    //NOTE: From Qt documentation:
    //Note that we do not use the Link and LinkVisited roles when rendering rich text in Qt, and that we recommend that you use CSS and the QTextDocument::setDefaultStyleSheet() function to alter the appearance of links.
    if(m_linkColor != color)
    {
        QString sheet = QString::fromLatin1("a { text-decoration: underline; color: %1 }").arg(color.name());
        document()->setDefaultStyleSheet(sheet);
        m_linkColor = color;
        RefreshUI();
        return true;
    }
    return false;
}

void QspTextBox::SetGamePath(const QString &path)
{
    m_path = path;
    setSearchPaths(QStringList(path));
}

void QspTextBox::SetBackgroundImage(const QPixmap& bmpBg)
{
    m_bmpBg = bmpBg;
    CalcImageSize();
}

//Returns the background color of the window.
QColor QspTextBox::GetBackgroundColor()
{
    return m_backColor;
}

//The meaning of foreground colour varies according to the window class; it may be the text colour or other colour, or it may not be used at all. Additionally, not all native controls support changing their foreground colour so this method may change their colour only partially or even not at all.
QColor QspTextBox::GetForegroundColor()
{
    return m_fontColor;
}

//Returns true if the color was really changed, false if it was already set to this color and nothing was done.
bool QspTextBox::SetBackgroundColor(const QColor &color)
{
    if(m_backColor != color)
    {
        QPalette p = palette();
        p.setColor(QPalette::Base, color);
        setPalette(p);
        m_backColor = color;
        setTextBackgroundColor(color);
        RefreshUI();
        return true;
    }
    return false;
}

bool QspTextBox::SetForegroundColor(const QColor &color)
{
    //TODO: find alternative
    //NOTE: From Qt documentation
    //Warning: Do not use this function (void 	setPalette(const QPalette &)) in conjunction with Qt Style Sheets.
    if(m_fontColor != color)
    {
        m_fontColor = color;
        setTextColor(color);
        QPalette p = palette();
        p.setColor(QPalette::Text, color);
        p.setColor(QPalette::WindowText, color);
        setPalette(p);
        RefreshUI();
        return true;
    }
    return false;
}

void QspTextBox::SetShowPlainText(bool isPlain)
{
    showPlainText = isPlain;
    RefreshUI();
}

void QspTextBox::CalcImageSize()
{
    if (!m_bmpBg.isNull())
    {
        int w, h;
        w = this->childrenRect().width();
        h = this->childrenRect().height();
        if (w < 1) w = 1;
        if (h < 1) h = 1;
        int srcW = m_bmpBg.width(), srcH = m_bmpBg.height();
        int destW = srcW * h / srcH, destH = srcH * w / srcW;
        if (destW > w)
            destW = w;
        else
            destH = h;
        m_posX = (w - destW) / 2;
        m_posY = (h - destH) / 2;
        if (destW > 0 && destH > 0)
            m_bmpRealBg = m_bmpBg.scaled(destW, destH);
    }
}

void QspTextBox::paintEvent(QPaintEvent *e)
{
    if (!m_bmpBg.isNull())
    {
        QPainter painter(viewport());
        painter.drawImage(m_posX, m_posY, *(new QImage(m_bmpRealBg.toImage())));
    }
    QTextBrowser::paintEvent(e);
}

void QspTextBox::resizeEvent(QResizeEvent *e)
{
    if (!m_bmpBg.isNull())
    {
        CalcImageSize();
    }
    QTextBrowser::resizeEvent(e);
}

QVariant QspTextBox::loadResource(int type, const QUrl &name)
{
#ifndef _WIN32
    QString new_name = QByteArray::fromPercentEncoding(name.toString().toUtf8());
    for(auto sPath : searchPaths())
    {
        QDir itDir(sPath);
        QDirIterator it(sPath, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
            if(new_name.compare(itDir.relativeFilePath(it.next()), Qt::CaseInsensitive) == 0)
                return QTextBrowser::loadResource(type, QUrl(itDir.relativeFilePath(it.filePath())));
    }
    return QTextBrowser::loadResource(type, QUrl(new_name));
#else
    return QTextBrowser::loadResource(type, name);
#endif
}
