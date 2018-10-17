#include "gthread.h"

TimeThread::TimeThread(QObject *parent):
    QThread(parent)
{
    //Initialize main timer (10ms +- 5% accuracy)
    MainTimer = new QTimer();
    MainTimer->setTimerType(Qt::CoarseTimer);
    MainTimer->setInterval(10);
    connect(MainTimer,      &QTimer::timeout,
            this,           &TimeThread::MainTimeoutHandle);
    //Initialize accurate timer (1ms accuracy)
    AccurateTimer = new QTimer();
    AccurateTimer->setTimerType(Qt::PreciseTimer);
    AccurateTimer->setInterval(1);
    connect(AccurateTimer,  &QTimer::timeout,
            this,           &TimeThread::AccurateTimeoutHandle);
}

TimeThread::~TimeThread()
{
    delete AccurateTimer;
    delete MainTimer;
}

void TimeThread::runMain()
{
    TimerCounter = 0;
    MainTimer->start();
    exec();
}

void TimeThread::MainTimeoutHandle()
{
    //qDebug()<<"Timeout Event"<<TimerCounter;
    if(TimerCounter >= 500)
        TimerCounter = 0;
    else
        TimerCounter++;
    emit Timeout10ms();
    if(TimerCounter % 5 == 0)
    {
        emit Timeout50ms();
        if(TimerCounter % 2 == 0)
            emit Timeout100ms();
    }
}

inline void TimeThread::stopMain(){
    MainTimer->stop();
}

void TimeThread::setTimeThread(bool startThread)
{
    if(startThread)
        this->runMain();
    else
        this->stopMain();
}

void TimeThread::AccurateTimeoutHandle(){
    emit Timeoutms();
}

void TimeThread::startTimeThread(){
    runMain();
}

void TimeThread::pauseTimeThread(){
    stopMain();
}

DataEngine::DataEngine(QObject *parent):
    QThread (parent)
{
    OriginData = new int[ChannelNum];
    int pos = 0;
    QFile myfile(":/spectral/data0.txt");
    myfile.open(QFile::ReadOnly);
    QTextStream textStream(&myfile);
    while(!textStream.atEnd())
    {
        QString Line = textStream.readLine();
        if(Line.isEmpty())
            continue;
        QStringList List = Line.split(QRegExp("[, \t;|]"));
        for(int counter = 0; counter < Line.length(); counter++, pos++)
        {
            if(pos != 0)
                OriginData[pos] = List.value(pos).toInt() + OriginData[pos - 1];
            else
                OriginData[pos] = List.value(pos).toInt();
        }
    }
    myfile.close();
}

void DataEngine::run()
{
    while(1)
    {
        long long int randbuff = 0;
        pause.lock();
        int counter;
        for(counter = 0; counter < 1000; counter++)
        {
            randbuff = (rand() % (OriginData[ChannelNum - 1]+1));
            for(counter = 0; counter < ChannelNum; counter++)
                if(randbuff <= OriginData[counter])
                    break;
            emit SimulateEvent(counter);
        }
        pause.unlock();
    }
}
