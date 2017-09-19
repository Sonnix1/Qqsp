#ifndef QSPINPUTBOX_H
#define QSPINPUTBOX_H

#include <QWidget>
#include <QTextEdit>
#include <QString>
#include <QStringList>

namespace Ui {
class QspInputBox;
}

//TODO: implement
class QspInputBox : public QTextEdit
{
    Q_OBJECT

public:
    explicit QspInputBox(QWidget *parent = 0);
    ~QspInputBox();

    // Accessors
    void SetText(const QString& text, bool isChangeValue = true);
    QString GetText() const { return m_text; }

private:
    // Fields
    QString m_text;
    QStringList m_strings;
    int m_selIndex;
};

#endif // QSPINPUTBOX_H
