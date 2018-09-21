#ifndef GPLOT_H
#define GPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <QtGui>
#include "gsetting.h"
#include "gdatabase.h"

class OverallPlot: public QwtPlot
{
    Q_OBJECT
public:
    explicit OverallPlot(QWidget *parent);
    ~OverallPlot();
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

};

#endif // GPLOT_H
