#include "qspwebchannel.h"

#include <QMessageBox>

#include <qsp_default.h>
#include "callbacks_gui.h"
#include "comtools.h"

QspWebChannel::QspWebChannel(QObject *parent) : QObject(parent)
{

}

void QspWebChannel::ExecString(const QString &string)
{
    if (!QSPExecString(qspStringFromQString(string), QSP_TRUE))
        ShowError();
}

void QspWebChannel::ShowError()
{
    QString errorMessage;
    QSP_CHAR *loc;
    int code, actIndex, line;
    QSPGetLastErrorData(&code, &loc, &actIndex, &line);
    QString desc = QSPTools::qspStrToQt(QSPGetErrorDesc(code));
    if (loc)
        errorMessage = QString("Location: %1\nArea: %2\nLine: %3\nCode: %4\nDesc: %5")
                .arg(QSPTools::qspStrToQt(loc))
                .arg(actIndex < 0 ? QString("on visit") : QString("on action"))
                .arg(line)
                .arg(code)
                .arg(desc);
    else
        errorMessage = QString("Code: %1\nDesc: %2")
                .arg(code)
                .arg(desc);
    QMessageBox dialog(QMessageBox::Critical, tr("Error"), errorMessage, QMessageBox::Ok);
    dialog.exec();
    QSPCallBacks::RefreshInt(QSP_FALSE);
}
