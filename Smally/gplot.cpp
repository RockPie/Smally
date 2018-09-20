#include "gplot.h"

OverallPlot::OverallPlot(QWidget *parent):
    QwtPlot(parent)
{
    InitCanvas();
}

OverallPlot::~OverallPlot()
{

}

void OverallPlot::InitCanvas()
{
    //Set background color
    this->canvas()->setPalette(QPalette(QColor(Qt::black)));
    setAutoFillBackground(true);
    //Set grid
    QwtPlotGrid *OverallGrid = new QwtPlotGrid;
    OverallGrid->enableXMin(true);
    OverallGrid->enableYMin(true);
    //Grid style
    OverallGrid->setMajorPen(QPen(Qt::white, 0, Qt::DotLine));
    OverallGrid->setMinorPen(QPen(Qt::gray , 0, Qt::DotLine));
    OverallGrid->attach(this);
    //Set Axes
    setAxisScale(QwtPlot::yLeft  , 0.0, OverallYIniMax);
    setAxisScale(QwtPlot::xBottom, 0.0, ChannelNum - 1);
    //setAutoReplot(true);
}
