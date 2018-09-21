#ifndef GDATABASE_H
#define GDATABASE_H

#include "gsetting.h"
#include <QtGui>
#include <QtCore/qmath.h>
#include <QDebug>

class Spectral
{
public:
    Spectral(const QString ele = QString(), const uint nucnum = 0);
    ~Spectral();
    void ReceiveCount(const uint Channel);
    const QVector <QPointF> PointOutput(bool isXlog = false, bool isYlog = false);
    void setSysCurve(uint InitChannel, uint64_t InitMax, uint64_t InitMin);
    void setSysCurve(uint64_t InitMax, uint64_t InitMin);
    void setSysCurve(uint InitChannel);
public:
    QString Element;
    uint NucleonNum;
    bool isSystemCurve;
private:
    uint64_t *CountingData;
    uint SysChannel;
};



#endif // GDATABASE_H
