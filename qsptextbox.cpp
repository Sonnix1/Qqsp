#include "qsptextbox.h"

#include <QFileInfo>
#include <QPalette>
#include <QAbstractScrollArea>
#include <QScrollBar>

#include "comtools.h"

QspTextBox::QspTextBox(QWidget *parent) : QTextBrowser(parent)
{
    //SetBorders(5);
    m_isUseHtml = false;
    showPlainText = false;
    m_linkColor = palette().color(QPalette::Link);
    setOpenLinks(false);
//	m_font = *wxNORMAL_FONT;
//	m_outFormat = wxString::Format(
//		wxT("<HTML><META HTTP-EQUIV = \"Content-Type\" CONTENT = \"text/html; charset=%s\">")
//		wxT("<BODY><FONT COLOR = #%%s>%%s</FONT></BODY></HTML>"),
//		wxFontMapper::GetEncodingName(wxLocale::GetSystemEncoding()).wx_str()
//	);
//	wxString fontName(m_font.GetFaceName());
//	SetStandardFonts(m_font.GetPointSize(), fontName, fontName);
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
    QString color(QSPTools::GetHexColor(GetForegroundColour()));
    QString text(QSPTools::HtmlizeWhitespaces(m_isUseHtml ? m_text : QSPTools::ProceedAsPlain(m_text)));
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

void QspTextBox::SetTextFont(const QFont& font)
{
    if (m_font != font)
    {
        m_font = font;
        setFont(font); //TODO: check which one of this to use
        setCurrentFont(font);
    }
}

bool QspTextBox::SetLinkColor(const QColor &colour)
{
    //QPalette p = palette();
    //p.setBrush( QPalette::Link, clr);
    //setPalette( p );
    if(m_linkColor != colour)
    {
        QString sheet = QString::fromLatin1("a { text-decoration: underline; color: %1 }").arg(colour.name());
        document()->setDefaultStyleSheet(sheet);
        m_linkColor = colour;
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

//TODO:
void QspTextBox::SetBackgroundImage(const QPixmap& bmpBg)
{
    m_bmpBg = bmpBg;
    //CalcImageSize();
}

//Returns the background colour of the window.
QColor QspTextBox::GetBackgroundColour()
{
    return textBackgroundColor();
}

//The meaning of foreground colour varies according to the window class; it may be the text colour or other colour, or it may not be used at all. Additionally, not all native controls support changing their foreground colour so this method may change their colour only partially or even not at all.
QColor QspTextBox::GetForegroundColour()
{
    return textColor();
}

//Returns true if the colour was really changed, false if it was already set to this colour and nothing was done.
bool QspTextBox::SetBackgroundColour(const QColor &colour)
{
    if(textBackgroundColor() != colour)
    {
        //QPalette p = palette();
        //p.setColor(QPalette::Base, colour);
        //setPalette(p);
        setTextBackgroundColor(colour);
        RefreshUI();
        return true;
    }
    return false;
}

bool QspTextBox::SetForegroundColour(const QColor &colour)
{
    if(textColor() != colour)
    {
        setTextColor(colour);
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

//void QSPTextBox::CalcImageSize()
//{
//    if (m_bmpBg.Ok())
//    {
//        int w, h;
//        GetClientSize(&w, &h);
//        if (w < 1) w = 1;
//        if (h < 1) h = 1;
//        int srcW = m_bmpBg.GetWidth(), srcH = m_bmpBg.GetHeight();
//        int destW = srcW * h / srcH, destH = srcH * w / srcW;
//        if (destW > w)
//            destW = w;
//        else
//            destH = h;
//        m_posX = (w - destW) / 2;
//        m_posY = (h - destH) / 2;
//        if (destW > 0 && destH > 0)
//            m_bmpRealBg = wxBitmap(m_bmpBg.ConvertToImage().Scale(destW, destH));
//        else
//            m_bmpRealBg = wxNullBitmap;
//    }
//}
