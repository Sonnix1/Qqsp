#ifndef QSPWEBENGINEURLREQUESTINTERCEPTOR_H
#define QSPWEBENGINEURLREQUESTINTERCEPTOR_H

#include <QObject>
#include <QWebEngineUrlRequestInterceptor>

class QspWebEngineUrlRequestInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT

public:
    explicit QspWebEngineUrlRequestInterceptor(QObject *parent = 0);
    virtual void interceptRequest(QWebEngineUrlRequestInfo &info) Q_DECL_OVERRIDE;

private:
};

#endif // QSPWEBENGINEURLREQUESTINTERCEPTOR_H
