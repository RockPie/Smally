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
    void TimeoutHandle();
    void setTimeThread(bool startThread);

protected:
    void run();
    inline void stop();

private:
    QTimer *MainTimer;
    volatile uint64_t TimerCounter;

signals:
    void Timeoutms();
    void Timeout10ms();
    void Timeout50ms();
    void Timeout100ms();

};

#endif // GTHREAD_H
