#include "qspinputbox.h"

QspInputBox::QspInputBox(QWidget *parent) : QTextEdit(parent)
{
    m_selIndex = -1;
}

QspInputBox::~QspInputBox()
{

}

void QspInputBox::SetText(const QString& text, bool isChangeValue)
{
    if (m_text != text)
    {
        m_text = text;
        //if (isChangeValue) ChangeValue(m_text); It also marks the control as not-modified which means that IsModified() would return false immediately after the call to ChangeValue().
        //The insertion point is set to the start of the control (i.e. position 0) by this function.
        //This functions does not generate the wxEVT_TEXT event but otherwise is identical to SetValue().
        if (isChangeValue) setText(m_text);
    }
}
