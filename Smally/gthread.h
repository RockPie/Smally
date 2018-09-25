#ifndef GTHREAD_H
#define GTHREAD_H

#include <QtGui>

class TimeThread: public QThread
{
    Q_OBJECT
public:
    explicit TimeThread(QObject *parent);
    ~TimeThread();

    void runTThread();

private:
    QTimer *MainTimer;

};

#endif // GTHREAD_H
