#include "qspnetworkaccessmanager.h"
#include "qspreply.h"

QspNetworkAccessManager::QspNetworkAccessManager(QNetworkAccessManager *oldManager, QObject *parent) : QNetworkAccessManager(parent)
{
    //setCache(oldManager->cache());
    setCookieJar(oldManager->cookieJar());
    //setProxy(oldManager->proxy());
    //setProxyFactory(oldManager->proxyFactory());
}

QspNetworkAccessManager::~QspNetworkAccessManager()
{

}

void QspNetworkAccessManager::SetPlainText(const QString &text)
{
    m_isUseHtml = false;
    m_text= text;
}

void QspNetworkAccessManager::SetHtml(const QString &text)
{
    m_isUseHtml = true;
    m_text =text;
}

void QspNetworkAccessManager::SetGamePath(const QString &path)
{
    m_path = path;
}

void QspNetworkAccessManager::SetLinkColor(const QColor &color)
{
    m_linkColor = color;
}

void QspNetworkAccessManager::SetBackgroundColor(const QColor &color)
{
    m_backColor = color;
}

void QspNetworkAccessManager::SetForegroundColor(const QColor &color)
{
    m_fontColor = color;
}

void QspNetworkAccessManager::SetBackgroundImage(const QString &path)
{
    m_bmpBg = path;
}

void QspNetworkAccessManager::SetTextFont(const QFont &new_font)
{
    m_font = new_font;
}

QNetworkReply *QspNetworkAccessManager::createRequest(
    QNetworkAccessManager::Operation operation, const QNetworkRequest &request,
    QIODevice *device)
{
    if (request.url().scheme() == "http" || request.url().scheme() == "https")
        return QNetworkAccessManager::createRequest(operation, request, device);

    if (operation == GetOperation)
        // Handle qsp:// URLs separately by creating custom QNetworkReply objects.
        return new QspReply(request.url(), m_text, m_isUseHtml, m_path, m_linkColor, m_backColor, m_fontColor, m_bmpBg, m_font);
    else
        return QNetworkAccessManager::createRequest(operation, request, device);
}
