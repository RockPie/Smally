#include "gthread.h"

TimeThread::TimeThread(QObject *parent):
    QThread(parent)
{

}

TimeThread::~TimeThread(){}

void TimeThread::runTThread()
{
    MainTimer = new QTimer();
    MainTimer->setInterval(500);
    MainTimer->start();
    exec();
}
