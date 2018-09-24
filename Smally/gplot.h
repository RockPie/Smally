#ifndef GPLOT_H
#define GPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <QtGui>
#include "gsetting.h"
#include "gcomponent.h"

class OverallPlot: public QwtPlot
{
    Q_OBJECT
public:
    explicit OverallPlot(QWidget *parent);
    ~OverallPlot();
    void OADataReceive(
            const QVector<QPointF> OAdata);
    DoubleSlider *OASlider;

private:
    void InitCanvas();
    void AddSysCurve();
    void AddMainCurve();

private:
    QVector <QwtPlotCurve> *CurveGroup;
    QwtPlotCurve *MainCurve;
    QwtPlotCurve *SysCurve;
    uint64_t *Limits;
    double SysCurveY[2];
    double SysCurveXmax[2];
    double SysCurveXmin[2];
    double InitMainX[ChannelNum];
    double InitMainY[ChannelNum];
    inline void SysCurveRefresh();

private slots:
    void MinSysChange(double val);
    void MaxSysChange(double val);
};

inline void OverallPlot::SysCurveRefresh()
{
    SysCurve[0].setSamples(SysCurveXmin, SysCurveY, 2);
    SysCurve[1].setSamples(SysCurveXmax, SysCurveY, 2);
    replot();
}

#endif // GPLOT_H
