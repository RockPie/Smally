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
    AccurateTimer->setInterval(10);
    connect(AccurateTimer,  &QTimer::timeout,
            this,           &TimeThread::AccurateTimeoutHandle);
}

TimeThread::~TimeThread()
{
    delete AccurateTimer;
    delete MainTimer;
    this->requestInterruption();
    this->quit();
    this->wait();
}

void TimeThread::runMain()
{
    TimerCounter = 0;
    MainTimer->start();
    AccurateTimer->start();
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
    if(TimerCounter % 2 == 0)
        emit Timeout20ms();
    if(TimerCounter % 5 == 0)
    {
        emit Timeout50ms();
        if(TimerCounter % 2 == 0)
            emit Timeout100ms();
    }
}

void TimeThread::setTimeThread(bool startThread)
{
    if(startThread)
        this->runMain();
    else
    {
        this->stopMain();
        this->stopAcc();
    }
}

void TimeThread::AccurateTimeoutHandle(){
    emit Timeoutms();
}

void TimeThread::startTimeThread(){
    runMain();
}

void TimeThread::pauseTimeThread(){
    this->stopMain();
    this->stopAcc();
}

void TimeThread::pauseAccurateThread(){
    this->stopAcc();
}

DataEngine::DataEngine(QObject *parent):
    QThread (parent)
  , DataNum(0)
{
    OriginData = new int*[5];
    for(int counter = 0; counter < 5; counter++)
        OriginData[counter] = new int[ChannelNum];
    ReadtoInt(":/spectral/bulid-in_spec/data0.txt", 0);
    ReadtoInt(":/spectral/bulid-in_spec/data1.txt", 1);
    ReadtoInt(":/spectral/bulid-in_spec/data2.txt", 2);
    ReadtoInt(":/spectral/bulid-in_spec/data3.txt", 3);
    ReadtoInt(":/spectral/bulid-in_spec/data4.txt", 4);
    pause.lock();
    this->start();

}

DataEngine::~DataEngine()
{
    this->requestInterruption();
    this->quit();
    this->wait();
}

void DataEngine::run()
{
    while(1)
    {
        int randbuff = 0;
        int subcounter = 0;
        pause.lock();
        QTime time;
        time= QTime::currentTime();
        qsrand(uint(time.msec()+time.second()*1000));
        for(int counter = 0; counter < 500; counter++)
        {
            randbuff = (qrand() % (OriginData[DataNum][ChannelNum - 1]));
            for(subcounter = 0; subcounter < ChannelNum; subcounter++)
                if(randbuff <= OriginData[DataNum][subcounter])
                    break;
            emit SimulateEvent(subcounter);
        }
        for(int counter = 0; counter < 250; counter++)
            emit SimulateEvent(qrand() % ChannelNum);
        msleep(10);
        pause.unlock();
    }
}

void DataEngine::ReadtoInt(QString fname, int pos)
{
    QFile myfile(fname);
    myfile.open(QFile::ReadOnly);
    QTextStream textStream(&myfile);
    while(!textStream.atEnd())
    {
        QString Line = textStream.readLine();
        if(Line.isEmpty())
            continue;
        QStringList List = Line.split(' ');
        Databuffer[0] = List.value(0).toInt();
        for(int counter = 1; counter < ChannelNum; counter++){
                Databuffer[counter] =
                List.value(counter).toInt() + Databuffer[counter - 1];
        }
    }
    int coe = int(Databuffer[ChannelNum - 1] / 10000);
    for(int counter = 0; counter < ChannelNum; counter++)
        OriginData[pos][counter] = int(Databuffer[counter] / coe);
    myfile.close();
}

void DataEngine::setDataNum(int input){
    DataNum = input;
}
