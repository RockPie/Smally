#ifndef GPLOT_H
#define GPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
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

public slots:
    void setDotDisplay(bool val);
    void setXLogMode(bool val);
    void setYLogMode(bool val);

public:
    PartPlot *AttachedPlot;
    DoubleSlider *OASlider;

};

/*
class PartPlot: public QwtPlot
{
    Q_OBJECT
public:
    explicit PartPlot(QWidget *parent);
    ~PartPlot();
    void PartDataReceive(
            const QVector<QPointF> OAdata);
    inline void setPPDots();
    inline void setPPLines();
    inline void setPPData(QVector <QPointF> input);

private:
    void InitCanvas();
    void AddPartCurve();

private:
    QwtPlotCurve *PartCurve;
    double InitPartX[ChannelNum];
    double InitPartY[ChannelNum];
};

inline void PartPlot::setPPDots(){
    PartCurve->setStyle(QwtPlotCurve::Dots);
}

inline void PartPlot::setPPLines(){
    PartCurve->setStyle(QwtPlotCurve::Lines);
}

inline void PartPlot::setPPData(QVector <QPointF> input){
    PartCurve->setSamples(input);
}
*/


/*
class OverallPlot: public QwtPlot
{
    Q_OBJECT
public:
    explicit OverallPlot(QWidget *parent);
    ~OverallPlot();
    void OADataReceive(
            const QVector<QPointF> OAdata);

    DoubleSlider *OASlider;
    PartPlot *AttachedPlot;

public slots:
    void setDotDisplay(bool isDot);
    void MinSysChange(double val);
    void MaxSysChange(double val);
    void setXLogMode(bool isLog);
    void setYLogMode(bool isLog);

private:
    void InitCanvas();
    void AddSysCurve();
    void AddMainCurve();

private:
    QwtPlotCurve *MainCurve;
    QwtPlotCurve *SysCurve;
    uint64_t *Limits;
    double SysCurveY[2];
    double SysCurveXmax[2];
    double SysCurveXmin[2];
    double InitMainX[ChannelNum];
    double InitMainY[ChannelNum];
    const double LogChannel =
            double(20) * qLn(double(ChannelNum));

    inline void SysCurveRefresh();
};

inline void OverallPlot::SysCurveRefresh()
{
    SysCurve[0].setSamples(SysCurveXmin, SysCurveY, 2);
    SysCurve[1].setSamples(SysCurveXmax, SysCurveY, 2);
    this->replot();
}
*/



#endif // GPLOT_H
