#ifndef GCLIENT_H
#define GCLIENT_H

#include <QtNetwork>
#include <QTextStream>
#include <QThread>

class GNetwork: public QThread
{
    Q_OBJECT
public:
    explicit GNetwork(QObject *parent);
    ~GNetwork();

public slots:
    void newConnect();
    void readMessage();
    void onDisconnected();
    void displayError(QAbstractSocket::SocketError);
    void setHostName(QString str);
    void setPortNum(QString str);
    void disconnect();

protected:
    virtual void run();

signals:
    void newConnectionSet();
    void messageReceived(QString message);
    void errorMet(QString errorInfo);
    void connectionDisabled();

private:
    QTcpSocket *GtcpSocket;
    QString GtcpMessage;
    quint16 BlockSize;

    QString hostName;
    quint16 portNum;

    bool isThreadStopped = false;
    bool isConnectionOk = false;

};

#endif // GCLIENT_H
