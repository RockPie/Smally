#ifndef GPLOT_H
#define GPLOT_H

#include <QtGui>
#include <QtCore/qmath.h>
#include "gsetting.h"
#include "gcomponent.h"
#include "gpaintingsystem.h"

class PartPlot: public GPlot
{
    Q_OBJECT
public:
    explicit PartPlot(QWidget *parent);
    ~PartPlot();
private:
    double InitPartX[ChannelNum];
    double InitPartY[ChannelNum];
};

class OverallPlot: public GPlot
{
    Q_OBJECT
public:
    explicit OverallPlot(QWidget *parent);
    ~OverallPlot();
    void OADataReceive(
            const QVector<QPointF> OAdata);
    inline void setTitle(QString title);

public slots:
    void setDotDisplay(bool val);
    void setXLogMode(bool isLog);
    void setYLogMode(bool isLog);
    void setAutoScale(bool isAuto);
    void setThreePoints(bool isTPF);
    void setFivePoints(bool isFPF);
    void setHighAccuracy(bool isHAF);

public:
    PartPlot *AttachedPlot;
    DoubleSlider *OASlider;

private:
    const double LogChannel =
            double(20) * qLn(double(ChannelNum));
};

inline void OverallPlot::setTitle(QString title){
    OASlider->setLabel(title);
}



#endif // GPLOT_H
