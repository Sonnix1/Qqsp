#include "qsplistbox.h"

#include <QListWidgetItem>
#include <QLabel>
#include <QList>
#include <QFileInfo>
#include <QScrollBar>
#include <QPalette>

#include "comtools.h"

#include "qsptextbox.h"

QspListBox::QspListBox(QWidget *parent) : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::NoSelection);
    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSizeAdjustPolicy(QListWidget::AdjustToContents);
    setContentsMargins(0,0,0,0);
    setSpacing(0);
    setFrameStyle(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    m_isUseHtml = false;
    m_isShowNums = false;
    showPlainText = false;
    //m_linkColor = palette().color(QPalette::Link);
    //m_textColor = palette().color(QPalette::Text);
    //m_backgroundColor = QColor(224, 224, 224);
    m_selectionColor = palette().color(QPalette::Highlight);
    m_font = font();
    oldSelection = -1;
    m_mouseTracking = false;
}

QspListBox::~QspListBox()
{

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

void QspListBox::SetTextFont(const QFont& new_font)
{
    if (m_font != new_font)
    {
        m_font = new_font;
        setFont(new_font);
        createList();
    }
}

bool QspListBox::SetLinkColor(const QColor &color)
{
    if(m_linkColor != color)
    {
        m_linkColor = color;
        createList();
        //RefreshUI();
        return true;
    }
    return false;
}

QColor QspListBox::GetLinkColor()
{
    return m_linkColor;
}

QColor QspListBox::GetBackgroundColor()
{
    return m_backgroundColor;
}

QColor QspListBox::GetForegroundColor()
{
    return m_textColor;
}

bool QspListBox::SetBackgroundColor(const QColor &color)
{
    if(m_backgroundColor != color)
    {
        QPalette p = palette();
        p.setColor(QPalette::Base, color);
        setPalette(p);
        m_backgroundColor = color;
        createList();
        return true;
    }
    return false;
}

bool QspListBox::SetForegroundColor(const QColor &color)
{
    if(m_textColor != color)
    {
        m_textColor = color;
        createList();
        return true;
    }
    return false;
}

void QspListBox::SetSelection(int selection)
{
    if(selection != oldSelection)
    {
        if(selection != -1 && selection < (count() - 1))
            if(item(selection) != 0)
                scrollToItem(item(selection));
        if(selection != -1)
        {
            QListWidgetItem *curItem =item(selection);
            if (curItem != 0)
                qobject_cast<QspTextBox*>(itemWidget(curItem))->SetBackgroundColor(m_selectionColor);
        }
        if(oldSelection != -1)
        {
            QListWidgetItem *curItem =item(oldSelection);
            if (curItem != 0)
                qobject_cast<QspTextBox*>(itemWidget(curItem))->SetBackgroundColor(m_backgroundColor);
        }
        oldSelection = selection;
        emit SelectionChange(selection);
    }
}

void QspListBox::SetShowPlainText(bool isPlain)
{
    showPlainText = isPlain;
    createList();
    RefreshUI();
}

void QspListBox::SetMouseTracking(bool trackMouse)
{
    m_mouseTracking = trackMouse;
    viewport()->setMouseTracking(trackMouse);
}

void QspListBox::createList()
{
    //clear(); //NOTE: clear() only deletes items but does not delete the widgets belonging to it. The widgets will be deleted if the QListWidget is deleted.
    //bool oldState = blockSignals(true);
    for(int i = 0; i<count(); i++)
    {
        removeItemWidget(item(i));
    }
    clear();
    for(int i = 0; i < qMin(m_images.size(), m_descs.size()); i++)
    {
        QString item_tmp;
        item_tmp = formatItem(i);

        QListWidgetItem* listItem;
        listItem = new QListWidgetItem(this);
        listItem->setBackground(m_backgroundColor);
        addItem(listItem);
        QspTextBox *item_widget;
        item_widget = new QspTextBox(this);
        //item_widget->setFrameStyle(QFrame::Box);
        item_widget->setLineWidth(0);
        item_widget->viewport()->setMouseTracking(false);
        item_widget->setAttribute(Qt::WA_TransparentForMouseEvents);

        item_widget->SetIsHtml(m_isUseHtml);
        item_widget->SetShowPlainText(showPlainText);
        item_widget->SetDisableVideo(true);

        item_widget->SetLinkColor(m_linkColor);
        item_widget->SetBackgroundColor(m_backgroundColor);
        item_widget->SetForegroundColor(m_textColor);
        item_widget->SetTextFont(m_font);

        item_widget->SetGamePath(m_path);
        //item_widget->setMaximumHeight(800);
        item_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        item_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        item_widget->verticalScrollBar()->setEnabled(false);
        item_widget->horizontalScrollBar()->setEnabled(false);
        item_widget->setReadOnly(true);
        item_widget->setBackgroundRole(QPalette::NoRole);
        item_widget->setTextInteractionFlags(Qt::NoTextInteraction);
        //item_widget->setWordWrapMode(QTextOption::NoWrap);
        item_widget->setWordWrapMode(QTextOption::WordWrap);
        //QFontMetrics font_metrics(item_widget->font());
        //item_widget->setFixedHeight(font_metrics.height() + 4* item_widget->frameWidth());
        item_widget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        item_widget->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
        item_widget->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);

        item_widget->SetText(item_tmp);

        //QSize sizehint = QSize(item->sizeHint().width(), font_metrics.height()*2 + item_widget->frameWidth()*2);
        //QSize sizehint = item_widget->sizeHint();
        //sizehint.setHeight(item_widget->document()->size().toSize().height());
        //sizehint.setHeight(item_widget->heightForWidth(this->width()));
        item_widget->document()->setTextWidth(this->width() - style()->pixelMetric(QStyle::PM_ScrollBarExtent) - 4 - item_widget->frameWidth()*4);
        QSize sizehint = QSize(this->width() - style()->pixelMetric(QStyle::PM_ScrollBarExtent) - 4 - item_widget->frameWidth()*4, item_widget->document()->size().toSize().height() + item_widget->frameWidth()*2);
        listItem->setSizeHint(sizehint);
        setItemWidget(listItem, item_widget);
    }
    if(oldSelection != -1)
    {
        QListWidgetItem *curItem =item(oldSelection);
        if (curItem != 0)
            qobject_cast<QspTextBox*>(itemWidget(curItem))->SetBackgroundColor(m_selectionColor);
    }
    adjustSize();
    //resizeEvent(0);
    //blockSignals(oldState);
}

QString QspListBox::formatItem(int itemIndex)
{
    if(itemIndex >= m_images.size() || itemIndex >= m_descs.size())
        return QString("");

    bool isImage = false;
    QString imgPath;
    if(!m_images.at(itemIndex).isEmpty())
    {
        QFileInfo imgFile(m_path + m_images.at(itemIndex));
        if (imgFile.exists() && imgFile.isFile())
        {
            imgPath = imgFile.absoluteFilePath();
            isImage = true;
        }
    }
    QString color(QSPTools::GetHexColor(GetForegroundColor()));
    QString formatedText;
    if (m_isShowNums && itemIndex < 9)
    {
        if (isImage)
        {
            return QString("<table cellspacing = 4 cellpadding = 0><td>[%1]</td><td><img src=\"%2\"></td><td WIDTH = 100%>%3</td></table>")
                    .arg(itemIndex+1)
                    .arg(imgPath)
                    .arg(m_descs.at(itemIndex));
        }
        else
        {
            return QString("<table cellspacing = 4 cellpadding = 0><tr><td>[%1]</td><td width = 100%>%2</td></td></table>")
                    .arg(itemIndex+1)
                    .arg(m_descs.at(itemIndex));
        }
    }
    else
    {
        if(isImage)
        {
            return QString("<table cellspacing = 4 cellpadding = 0><td><img src=\"%2\"></td><td WIDTH = 100%>%3</td></table>")
                    .arg(imgPath)
                    .arg(m_descs.at(itemIndex));
        }
        if(!m_descs.at(itemIndex).isEmpty())
        {
            return m_descs.at(itemIndex);
        }
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

void QspListBox::resizeEvent(QResizeEvent *e)
{
    for(int i = 0; i<count(); i++)
    {
        QListWidgetItem* listItem = item(i);
        if(listItem != 0)
        {
            QspTextBox *item_widget = qobject_cast<QspTextBox*>(itemWidget(listItem));
            if(item_widget != 0)
            {
                item_widget->document()->setTextWidth(this->width() - style()->pixelMetric(QStyle::PM_ScrollBarExtent) - 4 - item_widget->frameWidth()*4);
                QSize sizehint = QSize(this->width() - style()->pixelMetric(QStyle::PM_ScrollBarExtent) - 4 - item_widget->frameWidth()*4, item_widget->document()->size().toSize().height() + item_widget->frameWidth()*2);
                listItem->setSizeHint(sizehint);
            }
        }
    }
    QListWidget::resizeEvent(e);
}

void QspListBox::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseTracking)
    {
        QListWidgetItem *curItem = itemAt(event->pos());
        if (curItem != 0)
        {
            SetSelection(row(curItem));
        }
    }
    QListWidget::mouseMoveEvent(event);
}
