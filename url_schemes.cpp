#include "url_schemes.h"

#include <QWebEngineUrlScheme>

static const QByteArray QspWebEngineUrlSchemeName = QByteArrayLiteral("qsp");
static const QByteArray QspExecWebEngineUrlSchemeName = QByteArrayLiteral("exec");

void register_url_schemes()
{
    QWebEngineUrlScheme QspWebEngineUrlScheme(QspWebEngineUrlSchemeName);
    QspWebEngineUrlScheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);
    QspWebEngineUrlScheme.setFlags(QWebEngineUrlScheme::SecureScheme |
                                   QWebEngineUrlScheme::LocalAccessAllowed |
                                   QWebEngineUrlScheme::ServiceWorkersAllowed |
                                   QWebEngineUrlScheme::ViewSourceAllowed |
                                   QWebEngineUrlScheme::ContentSecurityPolicyIgnored);
    QWebEngineUrlScheme::registerScheme(QspWebEngineUrlScheme);
    QWebEngineUrlScheme QspExecWebEngineUrlScheme(QspExecWebEngineUrlSchemeName);
    QspExecWebEngineUrlScheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);
    QspExecWebEngineUrlScheme.setFlags(QWebEngineUrlScheme::SecureScheme |
                                   QWebEngineUrlScheme::LocalAccessAllowed |
                                   QWebEngineUrlScheme::ServiceWorkersAllowed |
                                   QWebEngineUrlScheme::ViewSourceAllowed |
                                   QWebEngineUrlScheme::ContentSecurityPolicyIgnored);
    QWebEngineUrlScheme::registerScheme(QspExecWebEngineUrlScheme);
}
