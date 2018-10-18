#include "gclient.h"

GNetwork::GNetwork(QObject *parent):
    QThread (parent){

}

GNetwork::~GNetwork()
{
    this->disconnect();
    this->quit();
    tcpSocket->deleteLater();
}

void GNetwork::readMessage()
{
    QDataStream inStream(tcpSocket);
    //Set Stream Version, which must be same as server
    inStream.setVersion(QDataStream::Qt_5_0);
    if(BlockSize == 0)  //if just begins
    {
        if(tcpSocket->bytesAvailable() < int(sizeof (quint16)))
            return;
        inStream>>BlockSize;
    }
    //If didn't get all the data
    if(tcpSocket->bytesAvailable() < BlockSize)
        return;
    inStream>>tcpMessage;
    emit messageReceived(tcpMessage);
}

void GNetwork::run()
{
    while(!isThreadStopped)
    {
        emit errorMet("Connecting...");
        if(!tcpSocket)
        {
            tcpSocket = new QTcpSocket();
            tcpSocket->abort();
            connect(tcpSocket,      &QTcpSocket::readyRead,
                    this,           &GNetwork::readMessage);
            connect(tcpSocket,      SIGNAL(error(QAbstractSocket::SocketError)),
                    this,           SLOT(displayError(QAbstractSocket::SocketError)));
            connect(tcpSocket,      &QTcpSocket::disconnected,
                    this,           &GNetwork::onDisconnected);
        }
        qDebug()<<"Running_2";
        if(!isConnectionOk)
        {
            BlockSize = 0;
            qDebug()<<"connect";
            tcpSocket->connectToHost(hostName, portNum, QIODevice::ReadOnly);
            qDebug()<<"Waiting";
            isConnectionOk = tcpSocket->waitForConnected(3000);
        }
        qDebug()<<"Running_3";
        if(!isConnectionOk)
            continue;
        tcpSocket->waitForReadyRead(5000);
        emit newConnectionSet();
    }
}

void GNetwork::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();
    emit errorMet(tcpSocket->errorString());
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
    //tcpSocket->abort();
    emit connectionDisabled();
}

