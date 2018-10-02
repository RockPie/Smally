#ifndef GDATABASE_H
#define GDATABASE_H

#include "gsetting.h"
#include <QtGui>
#include <QWidget>
#include <QtCore/qmath.h>

class Spectral: public QObject
{
    Q_OBJECT
public:
    Spectral(QWidget* parent, const QString ele = "Unknown",
             const uint nucnum = 0);
    ~Spectral();
    inline void ReceiveCount(const uint Channel);
    QVector <QPointF> PointOAOutput(
            bool isXlog = false, bool isYlog = false) const;
    QVector <QPointF> PointPartOutput(
            int StartPos, int Endpos,
            bool isXlog = false, bool isYlog = false) const;
    void Reset();
    inline double SignleOutput(int pos) const;
    inline void setChannel(int pos, uint64_t val);
    inline void setElement(QString ele);
    inline void setNucNum(uint nucnum);

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

inline double Spectral::SignleOutput(int pos) const{
    return double(CountingData[pos]);
}

inline void Spectral::setChannel(int pos, uint64_t val){
    CountingData[pos] = val;
}

inline void Spectral::setElement(QString ele){
    Element = ele;
}

inline void Spectral::setNucNum(uint nucnum){
    NucleonNum = nucnum;
}



#endif // GDATABASE_H
