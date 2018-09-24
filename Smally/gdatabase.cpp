#include "gdatabase.h"

Spectral::Spectral(const QString ele, const uint nucnum)
{
    Element = ele;
    NucleonNum = nucnum;
    CountingData = new uint64_t[ChannelNum];
    isSystemCurve = false;
}

Spectral::~Spectral()
{
    delete []CountingData;
}

//Output all counting info into QVector
//For QwtPlotCurve::setSamples(const QVector< QPointF > &samples)
QVector <QPointF> Spectral::PointOutput(
        bool isXlog, bool isYlog) const
{
    if(!isSystemCurve)
    {
        QVector <QPointF> res(ChannelNum);
        if(isXlog == false && isYlog == false)
            for(int counter = 0; counter < ChannelNum; counter++)
            {
                res[counter].setX(counter);
                res[counter].setY(CountingData[counter]);
            }
        else if(isXlog == true && isYlog == false)
            for(int counter = 0; counter < ChannelNum; counter++)
            {
                res[counter].setX(double(20)*(qLn(double(counter))));
                res[counter].setY(CountingData[counter]);
            }
        else if(isXlog == false && isYlog == true)
            for(int counter = 0; counter < ChannelNum; counter++)
            {
                res[counter].setX(counter);
                res[counter].setY(double(20)*(qLn(double(CountingData[counter]))));
            }
        else
            for(int counter = 0; counter < ChannelNum; counter++)
            {
                res[counter].setX(double(20)*(qLn(double(counter))));
                res[counter].setY(double(20)*(qLn(double(CountingData[counter]))));
            }
        return res;
    }
    else
    {
        QVector <QPointF> res(2);
        if(isXlog == false)
        {
            res[0].setX(SysChannel);
            res[1].setX(SysChannel);
        }
        else
        {
            res[0].setX(double(20)*(qLn(double(SysChannel))));
            res[1].setX(double(20)*(qLn(double(SysChannel))));
        }
        if(isYlog == false)
        {
            res[0].setY(CountingData[0]);
            res[1].setY(CountingData[1]);
        }
        else
        {
            res[0].setY(double(20)*(qLn(double(CountingData[0]))));
            res[1].setY(double(20)*(qLn(double(CountingData[1]))));
        }
        return res;
    }
}
