#ifndef GCLIENT_H
#define GCLIENT_H

#define TCP_START 0xfc
#define TCP_PAUSE 0xfe
#define TCP_CLEAR 0xff

#include <QtNetwork>
#include <QTextStream>
#include <QAbstractSocket>
#include <QThread>
#include <cmath>
#include "gsetting.h"

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

    //Slots for MainWindow Operation
    void setHostName(QString str);
    void setPortNum(QString str);
    void disconnect();

    void sendTCPstart();
    void sendTCPpause();
    void sendTCPclear();

protected:
    virtual void run();

signals:
    void newConnectionSet();
    void messageReceived(quint64 *val);
    void errorMet(QString errorInfo);
    void connectionDisabled();

private:
    QTcpSocket *GtcpSocket;
    QString GtcpMessage;
    char* readBuffer;
    quint64 *SpectralData;

    QString hostName;
    quint16 portNum;

    bool isThreadStopped = false;
    bool isConnectionOk = false;

};

#endif // GCLIENT_H
