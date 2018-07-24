#ifndef QSPWEBCHANNEL_H
#define QSPWEBCHANNEL_H

#include <QObject>

class QspWebChannel : public QObject
{
    Q_OBJECT
public:
    explicit QspWebChannel(QObject *parent = nullptr);
    Q_INVOKABLE void ExecString(const QString &string);
private:
    void ShowError();
};

#endif // QSPWEBCHANNEL_H
