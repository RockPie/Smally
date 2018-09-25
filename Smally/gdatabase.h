#ifndef GDATABASE_H
#define GDATABASE_H

#include "gsetting.h"
#include <QtGui>
#include <QWidget>
#include <QtCore/qmath.h>

class Spectral: public QWidget
{
    Q_OBJECT
public:
    Spectral(QWidget* parent, const QString ele = QString(),
             const uint nucnum = 0);
    ~Spectral();
    inline void ReceiveCount(const uint Channel);
    QVector <QPointF> PointOAOutput(
            bool isXlog = false, bool isYlog = false) const;
    QVector <QPointF> PointPartOutput(
            int StartPos, int Endpos,
            bool isXlog = false, bool isYlog = false) const;

public:
    QString Element;
    uint NucleonNum;

private:
    uint64_t *CountingData;
};

//Receive one signal
inline void Spectral::ReceiveCount(const uint Channel)
{
    if(Channel < ChannelNum)
        CountingData[Channel]++;
    else
        qDebug()<<"Input count out of range\n"
                  "Incorrect count val ="<<Channel;
}



#endif // GDATABASE_H
