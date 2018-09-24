#ifndef GDATABASE_H
#define GDATABASE_H

#include "gsetting.h"
#include <QtGui>
#include <QtCore/qmath.h>
#include <QDebug>

class Spectral
{
public:
    Spectral(const QString ele = QString(),
             const uint nucnum = 0);
    ~Spectral();
    inline void ReceiveCount(const uint Channel);
    QVector <QPointF> PointOutput(
            bool isXlog = false, bool isYlog = false) const;
    inline void setSysCurve(uint InitChannel,
                            uint64_t InitMax,
                            uint64_t InitMin);
    inline void setSysCurve(uint64_t InitMax,
                            uint64_t InitMin);
    inline void setSysCurve(uint InitChannel);

public:
    QString Element;
    uint NucleonNum;
    bool isSystemCurve;

private:
    uint64_t *CountingData;
    uint SysChannel;
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

inline void Spectral::setSysCurve(uint InitChannel,
                           uint64_t InitMax, uint64_t InitMin)
{
    isSystemCurve = true;
    CountingData[0] = InitMin;
    CountingData[1] = InitMax;
    SysChannel = InitChannel;
}

inline void Spectral::setSysCurve(uint64_t InitMax, uint64_t InitMin)
{
    CountingData[0] = InitMin;
    CountingData[1] = InitMax;
}

inline void Spectral::setSysCurve(uint InitChannel){
    SysChannel = InitChannel;
}



#endif // GDATABASE_H
