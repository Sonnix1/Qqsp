#ifndef QSPLISTBOX_H
#define QSPLISTBOX_H

#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QStringList>
#include <QFont>
#include <QColor>
#include <QResizeEvent>
#include <QMouseEvent>

namespace Ui {
class QspListBox;
}

class QspListBox : public QListWidget
{
    Q_OBJECT

signals:
   void SelectionChange(int selection);

public:
    explicit QspListBox(QWidget *parent = 0);
    ~QspListBox();

    // Methods
    void RefreshUI();
    void BeginItems();
    void AddItem(const QString& image, const QString& desc);
    void EndItems();

    // Accessors
    void SetIsHtml(bool isHtml);
    void SetIsShowNums(bool isShow);
    void SetTextFont(const QFont& new_font);
    QFont GetTextFont() const { return m_font; }
    bool SetLinkColor(const QColor &color);
    QColor GetLinkColor();
    QColor GetBackgroundColor();
    QColor GetForegroundColor(); //text color
    bool SetBackgroundColor(const QColor& color);
    bool SetForegroundColor(const QColor& color);
    void SetGamePath(const QString& path) { m_path = path; }
    void SetSelection(int selection);
    int GetSelection() { return oldSelection; }
    void SetShowPlainText(bool isPlain);
    void SetMouseTracking(bool trackMouse);

private:
    // Internal methods
    void createList();
    QString formatItem(int itemIndex);
    void resizeEvent(QResizeEvent *e);
    void mouseMoveEvent(QMouseEvent *event);

    // Fields
    QString m_outFormat;
    QString m_outFormatNums;
    QString m_outFormatImage;
    QString m_outFormatImageNums;
    bool m_isUseHtml;
    bool m_isShowNums;
    QString m_path;
    QFont m_font;
    QStringList m_images;
    QStringList m_descs;
    QStringList m_newImages;
    QStringList m_newDescs;
    QColor m_linkColor;
    QColor m_textColor;
    QColor m_backgroundColor;
    QColor m_selectionColor;
    bool m_mouseTracking;
    bool showPlainText;
    int oldSelection;
};

#endif // QSPLISTBOX_H
