#ifndef GDATABASE_H
#define GDATABASE_H

#include "gsetting.h"
#include <QString>
#include <QObject>

class Spectral:
{
public:
    Spectral(const QString ele = QString(), const uint nucnum = 0);
    ~Spectral();
    void ReceiveCount(const uint Channel);
public:
    QString Element;
    uint NucleonNum;
private:
    uint64_t *CountingData;
};

#endif // GDATABASE_H
