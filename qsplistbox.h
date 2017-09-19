#ifndef QSPLISTBOX_H
#define QSPLISTBOX_H

#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QStringList>
#include <QFont>
#include <QColor>

namespace Ui {
class QspListBox;
}

class QspListBox : public QListWidget
{
    Q_OBJECT

public:
    explicit QspListBox(QWidget *parent = 0);
    ~QspListBox();

    // Methods
    void SetStandardFonts(const QFont& font);
    void RefreshUI();
    void BeginItems();
    void AddItem(const QString& image, const QString& desc);
    void EndItems();

    // Accessors
    void SetIsHtml(bool isHtml);
    void SetIsShowNums(bool isShow);
    void SetTextFont(const QFont& font);
    QFont GetTextFont() const { return m_font; }
    void SetLinkColor(const QColor& clr);
    QColor GetLinkColor();
    QColor GetTextColour(); //GetForegroundColour()
    void SetGamePath(const QString& path) { m_path = path; }
    void SetSelection(int selection);
    void SetShowPlainText(bool isPlain);

private:
    // Internal methods
    void createList();
    QString formatItem(int itemIndex);

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
    bool showPlainText;
};

#endif // QSPLISTBOX_H
