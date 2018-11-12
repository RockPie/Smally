#include "gclient.h"

GNetwork::GNetwork(QObject *parent)
    :QThread (parent)
    ,GtcpMessage(QString())
    ,hostName(QString())
    ,portNum(0)   {
    readBuffer = new char[8*ChannelNum];
    SpectralData = new quint64[ChannelNum];
}

GNetwork::~GNetwork()
{
    delete []SpectralData;
    delete []readBuffer;
    this->disconnect();
    this->requestInterruption();
    this->quit();
    this->wait();
}

void GNetwork::readMessage()
{
    GtcpSocket->read(readBuffer, 8*ChannelNum);
    for(int counter = 0; counter < ChannelNum; counter++)
    {
        SpectralData[counter] = 0;
        for(int subcounter = 0; subcounter < 8; subcounter++)
        {
            SpectralData[counter] +=
            readBuffer[counter * 8 + subcounter] * pow(10, subcounter);
        }
        //qDebug()<<SpectralData[counter];
    }
    emit messageReceived(SpectralData);
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
    //When user start connection
    while(!isThreadStopped)
    {
        //Turn into Connection Mode
        emit errorMet("Connecting...");
        //If TCPSocket is deleted
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
 //       qDebug()<<"Running_2";
        //If there is no connection set
        if(!isConnectionOk)
        {
//            qDebug()<<"connect";
            GtcpSocket->connectToHost(hostName, portNum);
            qDebug()<<"Waiting...";
            isConnectionOk = GtcpSocket->waitForConnected(3000);
        }
//        qDebug()<<"Running_3";
        //Retry
        if(!isConnectionOk)
            continue;
        //Send Success Signal
        emit newConnectionSet();
        //Wait for Data Input
        GtcpSocket->waitForReadyRead(3000);
        //After this we may get error signal because of timeout event
    }
}

void GNetwork::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<GtcpSocket->errorString();
    emit errorMet(GtcpSocket->errorString());
}

void GNetwork::setHostName(QString str){
    hostName = str;
//    qDebug()<<str;
}

void GNetwork::setPortNum(QString str){
    portNum = quint16(str.toUInt());
//    qDebug()<<portNum;
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
    GtcpSocket->deleteLater();
    isThreadStopped = true;
    msleep(1000);
    emit connectionDisabled();
}

void GNetwork::sendTCPstart(){
    char buf = char(TCP_START);
    GtcpSocket->write(&buf, 1);
}

void GNetwork::sendTCPpause(){
    char buf = char(TCP_PAUSE);
    GtcpSocket->write(&buf, 1);
}

void GNetwork::sendTCPclear(){
    char buf = char(TCP_CLEAR);
    GtcpSocket->write(&buf, 1);
}

