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
    TimerCounter = 0;
    MainTimer->setInterval(1);
    MainTimer->start();
    connect(MainTimer,      &QTimer::timeout,
            this,           &TimeThread::TimeoutHandle);
    exec();
}

void TimeThread::TimeoutHandle()
{
    //qDebug()<<"Timeout Event"<<TimerCounter;
    if(TimerCounter >= 500)
        TimerCounter = 0;
    else
        TimerCounter++;
    emit Timeoutms();
    if(!TimerCounter % 100)
        emit Timeout100ms();
    if(!TimerCounter % 50)
        emit Timeout50ms();
    if(!TimerCounter % 10)
        emit Timeout10ms();
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

