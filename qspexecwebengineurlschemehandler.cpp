#include "qspexecwebengineurlschemehandler.h"

#include <QString>
#include <QBuffer>
#include <QUrl>
#include <QMessageBox>

#include <qsp_default.h>
#include "callbacks_gui.h"
#include "comtools.h"

QspExecWebEngineUrlSchemeHandler::QspExecWebEngineUrlSchemeHandler(QObject *parent) : QWebEngineUrlSchemeHandler(parent)
{

}

void QspExecWebEngineUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request)
{
    url = request->requestUrl();
    QTimer::singleShot(0, this, SLOT(QspLinkClicked()));
}

void QspExecWebEngineUrlSchemeHandler::QspLinkClicked()
{
    emit qspLinkClicked(url);
}

void QspExecWebEngineUrlSchemeHandler::legacyLinkClicked(QWebEngineUrlRequestJob *request)
{
    url = request->requestUrl();
    QString href;
    href = QByteArray::fromPercentEncoding(url.toString().toUtf8());
    QString string = href.mid(5);
    if (!QSPExecString(qspStringFromQString(string), QSP_TRUE))
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
    //request->redirect(QUrl("qsp:/"));
}
