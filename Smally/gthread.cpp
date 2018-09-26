#include "gthread.h"

TimeThread::TimeThread(QObject *parent):
    QThread(parent)
{
}

TimeThread::~TimeThread()
{
    stop();
}

void TimeThread::run()
{
    MainTimer = new QTimer();
    MainTimer->setInterval(10);
    MainTimer->start();
    connect(MainTimer,      &QTimer::timeout,
            this,           &TimeThread::TimeoutHandle);
    exec();
}

void TimeThread::TimeoutHandle()
{
    qDebug()<<"Timeout Event";
    emit msecTimeout();
    //MainTimer->stop();
}

inline void TimeThread::stop()
{
    MainTimer->stop();
    delete MainTimer;
}

void TimeThread::setTimeThread(bool startThread)
{
    if(startThread)
        this->run();
    else
        this->stop();
}

