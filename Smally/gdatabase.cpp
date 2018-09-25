#include "gdatabase.h"

Spectral::Spectral(QWidget* parent, const QString ele,
                   const uint nucnum):
    QWidget(parent)
{
    Element = ele;
    NucleonNum = nucnum;
    CountingData = new uint64_t[ChannelNum];
}

Spectral::~Spectral()
{
    delete []CountingData;
}

//Output all counting info into QVector
//For QwtPlotCurve::setSamples(const QVector< QPointF > &samples)
QVector <QPointF> Spectral::PointOAOutput(
        bool isXlog, bool isYlog) const
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

QVector <QPointF> Spectral::PointPartOutput(
        int StartPos, int Endpos, bool isXlog, bool isYlog) const
{
    QVector <QPointF> res(Endpos - StartPos + 1);
    if(isXlog == false && isYlog == false)
        for(int counter = StartPos - 1; counter < Endpos; counter++)
        {
            res[counter  + 1 - StartPos].setX(counter);
            res[counter  + 1 - StartPos].setY(CountingData[counter]);
        }
    else if(isXlog == true && isYlog == false)
        for(int counter = StartPos - 1; counter < Endpos; counter++)
        {
            res[counter  + 1 - StartPos].
                    setX(double(20)*(qLn(double(counter))));
            res[counter  + 1 - StartPos].setY(CountingData[counter]);
        }
    else if(isXlog == false && isYlog == true)
        for(int counter = StartPos - 1; counter < Endpos; counter++)
        {
            res[counter  + 1 - StartPos].setX(counter);
            res[counter  + 1 - StartPos].
                    setY(double(20)*(qLn(double(CountingData[counter]))));
        }
    else
        for(int counter = StartPos - 1; counter < Endpos; counter++)
        {
            res[counter  + 1 - StartPos].
                    setX(double(20)*(qLn(double(counter))));
            res[counter  + 1 - StartPos].
                    setY(double(20)*(qLn(double(CountingData[counter]))));
        }
    return res;
}
