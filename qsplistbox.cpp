#include "qsplistbox.h"

#include <QListWidgetItem>
#include <QLabel>
#include <QList>
#include <QFileInfo>
#include <QScrollBar>

#include "comtools.h"

#include "qsptextbox.h"

QspListBox::QspListBox(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    m_isUseHtml = false;
    m_isShowNums = false;
    showPlainText = false;
//	SetStandardFonts(m_font.GetPointSize(), fontName, fontName);
//	SetSelectionBackground(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
}

QspListBox::~QspListBox()
{

}

void QspListBox::SetStandardFonts(const QFont &font)
{
    //	RefreshUI();
}

void QspListBox::RefreshUI()
{
//	RefreshAll();
}

void QspListBox::BeginItems()
{
    m_newImages.clear();
    m_newDescs.clear();
}

void QspListBox::AddItem(const QString& image, const QString& desc)
{
    m_newImages.append(image);
    m_newDescs.append(desc);
}

void QspListBox::EndItems()
{
    size_t count;
    if (m_images != m_newImages || m_descs != m_newDescs)
    {
        m_images = m_newImages;
        m_descs = m_newDescs;
        count = m_descs.count();
        createList();
        RefreshUI();
        if (count) scrollToItem(item(0));
    }
}

void QspListBox::SetIsHtml(bool isHtml)
{
    if (m_isUseHtml != isHtml)
    {
        m_isUseHtml = isHtml;
        createList();
        RefreshUI();
    }
}

void QspListBox::SetIsShowNums(bool isShow)
{
    if (m_isShowNums != isShow)
    {
        m_isShowNums = isShow;
        createList();
        RefreshUI();
    }
}

void QspListBox::SetTextFont(const QFont& font)
{
//	int fontSize = font.GetPointSize();
//	wxString fontName(font.GetFaceName());
//	if (!m_font.GetFaceName().IsSameAs(fontName, false) || m_font.GetPointSize() != fontSize)
//	{
//		m_font = font;
//		SetStandardFonts(fontSize, fontName, fontName);
//	}
}

void QspListBox::SetLinkColor(const QColor& clr)
{
//	RefreshUI();
}

QColor QspListBox::GetLinkColor()
{
    return m_linkColor;
}

QColor QspListBox::GetTextColour()
{
    return QColor(Qt::black);
}

void QspListBox::SetSelection(int selection)
{
    if(selection == -1)
        clearSelection();
    else
    {
        return;
        if(selection < count())
        {
            if(item(selection) != 0)
            {
                clearSelection();
                setCurrentItem(item(selection), QItemSelectionModel::SelectCurrent);
                item(selection)->setSelected(true);
                scrollToItem(item(selection));
                //setFocus();//TODO: check if required
            }
        }
        else
        {
            clearSelection();
            //clearFocus();
        }
    }
}

void QspListBox::SetShowPlainText(bool isPlain)
{
    showPlainText = isPlain;
    createList();
    RefreshUI();
}

void QspListBox::createList()
{
    //clear(); //NOTE: clear() only deletes items but does not delete the widgets belonging to it. The widgets will be deleted if the QListWidget is deleted.
    bool oldState = blockSignals(true);
    int selection = -1;
    QList<QListWidgetItem *> selList = selectedItems();
    if(selList.size() != 0)
        selection = row(selList.at(0));
    for(int i = 0; i<count(); i++)
    {
        removeItemWidget(item(i));
    }
    clear();
    for(int i = 0; i < qMin(m_images.size(), m_descs.size()); i++)
    {
        QString item_tmp;
        item_tmp = formatItem(i);

        QListWidgetItem* item;
        item = new QListWidgetItem(this);
        addItem(item);
        QspTextBox *item_widget;
        item_widget = new QspTextBox(this);
        item_widget->SetIsHtml(m_isUseHtml);
        item_widget->SetGamePath(m_path);
        //item_widget->setMaximumHeight(800);
        item_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        item_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        item_widget->verticalScrollBar()->setEnabled(false);
        item_widget->horizontalScrollBar()->setEnabled(false);
        item_widget->setReadOnly(true);
        item_widget->setBackgroundRole(QPalette::NoRole);
        item_widget->setTextInteractionFlags(Qt::NoTextInteraction);
        item_widget->setWordWrapMode(QTextOption::NoWrap);
        QFontMetrics font_metrics(item_widget->font());
        //item_widget->setFixedHeight(font_metrics.height() + 4* item_widget->frameWidth());
        item_widget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        item_widget->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        item_widget->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);

        item_widget->SetShowPlainText(showPlainText);
        item_widget->SetText(item_tmp);

        //QSize sizehint = QSize(item->sizeHint().width(), font_metrics.height()*2 + item_widget->frameWidth()*2);
        //QSize sizehint = item_widget->sizeHint();
        //sizehint.setHeight(item_widget->document()->size().toSize().height());
        //sizehint.setHeight(item_widget->heightForWidth(this->width()));
        item_widget->document()->setTextWidth(this->width() - style()->pixelMetric(QStyle::PM_ScrollBarExtent) - 4 - item_widget->frameWidth()*4);
        QSize sizehint = QSize(this->width() - style()->pixelMetric(QStyle::PM_ScrollBarExtent) - 4 - item_widget->frameWidth()*4, item_widget->document()->size().toSize().height() + item_widget->frameWidth()*2);
        item->setSizeHint(sizehint);
        setItemWidget(item, item_widget);
    }
    if(selection != -1)
        SetSelection(selection);
    blockSignals(oldState);
}

QString QspListBox::formatItem(int itemIndex)
{
    if(itemIndex >= m_images.size() || itemIndex >= m_descs.size())
        return QString("");

    bool isImage = false;
    QString imgPath;
    if(!m_images.at(itemIndex).isEmpty())
    {
        QFileInfo imgFile(m_images.at(itemIndex));
        if (imgFile.exists() && imgFile.isFile())
        {
            imgPath = imgFile.absoluteFilePath();
            isImage = true;
        }
    }
    QString color(QSPTools::GetHexColor(GetTextColour()));
    QString formatedText;
    if(isImage)
    {
        formatedText.append(QString("<img src=\"%1\">").arg(m_images.at(itemIndex)));
    }
    if(!m_descs.at(itemIndex).isEmpty())
    {
        formatedText.append(m_descs.at(itemIndex));
    }
    return formatedText;

    //TODO: make this variant work
    if(m_descs.at(itemIndex).isEmpty())
    {
        formatedText = "";
    }
    else
    {
        QString text(QSPTools::HtmlizeWhitespaces(m_isUseHtml ? m_descs.at(itemIndex) : QSPTools::ProceedAsPlain(m_descs.at(itemIndex))));
        formatedText = QString("<div style=\"padding:0px; margin-right:4px;\">%1</div>").arg(text);
        formatedText = m_descs.at(itemIndex);
    }

    if (m_isShowNums && itemIndex < 9)
    {
        if (isImage)
        {
            return QString("<div style=\"color : #%1; -qt-block-indent:0; text-indent:0px;\"><div style=\"padding:0px; margin-right:4px;\">[%2]</div><div style=\"padding:0px; margin-right:4px;\"><img src=\"%3\"></div>%4</div>")
                    .arg(color)
                    .arg(itemIndex+1)
                    .arg(imgPath)
                    .arg(formatedText);
        }
        else
        {
            return QString("<div style=\"color : #%1; -qt-block-indent:0; text-indent:0px;\"><div style=\"padding:0px; margin-right:4px;\">[%2]</div>%3</div>")
                    .arg(color)
                    .arg(itemIndex+1)
                    .arg(formatedText);
        }
    }
    else
    {
        if (isImage)
        {
            return QString("<div style=\"color : #%1; -qt-block-indent:0; text-indent:0px;\"><div style=\"padding:0px; margin-right:4px;\"><img src=\"%2\"></div>%3</div>")
                    .arg(color)
                    .arg(imgPath)
                    .arg(formatedText);
        }
        else
        {
            return QString("<div style=\"color : #%1; -qt-block-indent:0; text-indent:0px;\">%2</div>")
                    .arg(color)
                    .arg(formatedText);
        }
    }
}
