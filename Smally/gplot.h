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
    void AddCouningCurve(const Spectral *SpectralData);
private:
    void InitCanvas();
    void AddSysCurve();
private:
    QVector <QwtPlotCurve> *CurveGroup;

};

#endif // GPLOT_H
