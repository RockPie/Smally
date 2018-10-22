#ifndef GTHREAD_H
#define GTHREAD_H

#include <QtGui>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTime>
#include "gsetting.h"

class TimeThread: public QThread
{
    Q_OBJECT
public:
    explicit TimeThread(QObject *parent);
    ~TimeThread();
    inline void stopMain();
    inline void stopAcc();

public slots:
    void MainTimeoutHandle();
    void AccurateTimeoutHandle();
    void setTimeThread(bool startThread);
    void startTimeThread();
    void pauseTimeThread();

protected:
    void runMain();

private:
    QTimer *MainTimer;
    QTimer *AccurateTimer;
    volatile int TimerCounter;

signals:
    void Timeoutms();
    void Timeout10ms();
    void Timeout50ms();
    void Timeout100ms();

};

inline void TimeThread::stopMain(){
    MainTimer->stop();
}

inline void TimeThread::stopAcc(){
    AccurateTimer->stop();
}

class DataEngine: public QThread
{
    Q_OBJECT
public:
    explicit DataEngine(QObject *parent = nullptr);
    ~DataEngine();
    QMutex pause;

public slots:
    void setDataNum(int input);

protected:
    void run();

private:
    void ReadtoInt(QString fname, int pos);
    int **OriginData;
    int DataNum;
    long long int Databuffer[ChannelNum];

signals:
    void SimulateEvent(int Channel);
};

#endif // GTHREAD_H
