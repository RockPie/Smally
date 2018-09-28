#ifndef GTHREAD_H
#define GTHREAD_H

#include <QtGui>

class TimeThread: public QThread
{
    Q_OBJECT
public:
    explicit TimeThread(QObject *parent);
    ~TimeThread();

public slots:
    void MainTimeoutHandle();
    void AccurateTimeoutHandle();
    void setTimeThread(bool startThread);

protected:
    void runMain();
    inline void stopMain();

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
