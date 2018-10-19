#include "gclient.h"

GNetwork::GNetwork(QObject *parent)
    :QThread (parent)
    ,GtcpMessage(QString())
    ,BlockSize(0)
    ,hostName(QString())
    ,portNum(0)   {
}

GNetwork::~GNetwork()
{
    this->disconnect();
    this->requestInterruption();
    this->quit();
    this->wait();
}

void GNetwork::readMessage()
{
    /*
    QDataStream inStream(GtcpSocket);
    //Set Stream Version, which must be same as server
    inStream.setVersion(QDataStream::Qt_5_0);
    if(BlockSize == 0)  //if just begins
    {
        if(GtcpSocket->bytesAvailable() < int(sizeof (quint16)))
            return;
        inStream>>BlockSize;
    }
    //If didn't get all the data
    if(GtcpSocket->bytesAvailable() < BlockSize)
        return;
    inStream>>GtcpMessage;*/
    QByteArray buffer;
    buffer = GtcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        GtcpMessage += tr(buffer);
    }
    qDebug()<<GtcpMessage;
    emit messageReceived(GtcpMessage);
}

void GNetwork::run()
{
    //Make Sure GtcpSocket is Created
    GtcpSocket = new QTcpSocket();
    GtcpSocket->abort();
    connect(GtcpSocket,     &QTcpSocket::readyRead,
            this,           &GNetwork::readMessage);
    connect(GtcpSocket,     QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this,           &GNetwork::displayError, Qt::DirectConnection);
    connect(GtcpSocket,     &QTcpSocket::disconnected,
            this,           &GNetwork::onDisconnected);
    while(!isThreadStopped)
    {
        emit errorMet("Connecting...");
        if(!GtcpSocket)
        {
            GtcpSocket = new QTcpSocket();
            GtcpSocket->abort();
            connect(GtcpSocket,     &QTcpSocket::readyRead,
                    this,           &GNetwork::readMessage);
            connect(GtcpSocket,     QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                    this,           &GNetwork::displayError, Qt::DirectConnection);
            connect(GtcpSocket,     &QTcpSocket::disconnected,
                    this,           &GNetwork::onDisconnected);
        }
        qDebug()<<"Running_2";
        if(!isConnectionOk)
        {
            BlockSize = 0;
            qDebug()<<"connect";
            GtcpSocket->connectToHost(hostName, portNum, QIODevice::ReadOnly);
            qDebug()<<"Waiting...";
            isConnectionOk = GtcpSocket->waitForConnected(3000);
        }
//        qDebug()<<"Running_3";
        if(!isConnectionOk)
            continue;
        emit newConnectionSet();
        GtcpSocket->waitForReadyRead(5000);

        //sleep(1000);
    }
}

void GNetwork::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<GtcpSocket->errorString();
    emit errorMet(GtcpSocket->errorString());
}

void GNetwork::setHostName(QString str){
    hostName = str;
    qDebug()<<str;
}

void GNetwork::setPortNum(QString str){
    portNum = quint16(str.toUInt());
    qDebug()<<portNum;
}

void GNetwork::onDisconnected(){
    GtcpSocket->deleteLater();
    isConnectionOk = false;
}

void GNetwork::newConnect()
{
    isThreadStopped = false;
    this->start();
}

void GNetwork::disconnect()
{
    isThreadStopped = true;
    emit connectionDisabled();
}

