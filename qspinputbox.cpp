#include "qspinputbox.h"

QspInputBox::QspInputBox(QWidget *parent) : QPlainTextEdit(parent)
{
    m_selIndex = -1;
}

QspInputBox::~QspInputBox()
{

}

void QspInputBox::SetText(const QString& text)
{
    bool oldState = blockSignals(true);
    setPlainText(text);
    blockSignals(oldState);
}

QString QspInputBox::GetText()
{
    return toPlainText();
}

void QspInputBox::keyPressEvent(QKeyEvent *event)
{
    if ((event->key()==Qt::Key_Return || event->key() == Qt::Key_Enter) && (event->modifiers()==Qt::ControlModifier))
    {
        appendPlainText("\n");
        return;
    }
    else if (event->key()==Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        emit InputTextEnter();
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}
