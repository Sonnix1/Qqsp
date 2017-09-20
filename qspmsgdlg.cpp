#include "qspmsgdlg.h"

#include <QRect>

#include "mainwindow.h"

#include "callbacks_gui.h"
#include "comtools.h"

QspMsgDlg::QspMsgDlg(QWidget *parent) : QDialog(parent)
{

}

QspMsgDlg::~QspMsgDlg()
{

}

QspMsgDlg::QspMsgDlg(const QString &caption, const QString &text, QWidget *parent) : QDialog(parent) {
    setWindowTitle(caption);
    sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    connect(&m_desc, SIGNAL(anchorClicked(QUrl)), this, SLOT(OnLinkClicked(QUrl)));
    m_desc.setHtml(text);
    m_desc.sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_desc.sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    connect(&okButton, SIGNAL(clicked()), this, SLOT(close()));
    okButton.setGeometry(QRect(10, 130, 100, 20));
    okButton.setText(tr("OK"));
    resize(320,240);
    layout.addWidget(&m_desc);
    layout.addWidget(&okButton);
    setLayout(&layout);
    setModal(true);
    //exec();
}

QspMsgDlg::QspMsgDlg(const QColor& backColor,
                     const QColor& fontColor,
                     const QFont& font,
                     const QString& caption,
                     const QString& text,
                     bool isHtml,
                     const QString& gamePath,
                     QWidget *parent) : QDialog(parent)
{
    //SetBackgroundColour(backColor);
    m_desc.SetGamePath(gamePath);
    m_desc.SetIsHtml(isHtml);
    m_desc.SetBackgroundColour(backColor);
    m_desc.SetForegroundColour(fontColor);
    m_desc.SetTextFont(font);
    m_desc.SetText(text);
    //#ifdef __WXMSW__
    //	btnOk->SetBackgroundColour(backColor);
    //	btnOk->SetForegroundColour(fontColor);
    //#endif
    // ----------
    setWindowTitle(caption);
    sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    m_desc.sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
    m_desc.sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
    connect(&okButton, SIGNAL(clicked()), this, SLOT(close()));
    okButton.setGeometry(QRect(10, 130, 100, 20));
    okButton.setText("OK");
    okButton.setDefault(true);
    okButton.setFont(font);
    okButton.setFocus(Qt::PopupFocusReason);

    resize(450,100);

    layout.addWidget(&m_desc);
    layout.addWidget(&okButton);
    setLayout(&layout);
    setModal(true);
    //exec();
}

void QspMsgDlg::OnLinkClicked(const QUrl &url)
{
    QString href;
    href = url.toString();

    if (href.startsWith("#"))
    {
        m_desc.setSource(url);
    }
    else if (href.toUpper().startsWith("EXEC:", Qt::CaseInsensitive)) //NOTE: was not part of original player
    {
        QString string = href.mid(5);
        if (!QSPExecString(qspStringFromQString(string), QSP_TRUE))
            if(this->parent() != 0)
                qobject_cast<MainWindow*>(this->parent())->ShowError(); //TODO: replace with signal
    }
    else
    {
        //LaunchDefaultBrowser(url); //TODO: this
    }
}
