#ifndef GTHREAD_H
#define GTHREAD_H

#include <QtGui>

class TimeThread: public QThread
{
    Q_OBJECT
public:
    explicit TimeThread(QObject *parent);
    ~TimeThread();
    inline void stopMain();

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

#endif // GTHREAD_H
