#include "gplot.h"

OverallPlot::OverallPlot(QWidget *parent):
    QwtPlot(parent)
{
    Limits = new uint64_t[2];
    Limits[0] = uint64_t(0);
    Limits[1] = uint64_t(OverallYIniMax);
    SysCurveY[0] = double(Limits[0]) - double(Limits[1]) * 0.2;
    SysCurveY[1] = 1.2 * double(Limits[1]);
    SysCurveXmin[0] = OAMinLinePos;
    SysCurveXmin[1] = OAMinLinePos;
    SysCurveXmax[0] = OAMaxLinePos;
    SysCurveXmax[1] = OAMaxLinePos;
    for(int counter = 0; counter < ChannelNum; counter++)
    {
        InitMainX[counter] = counter;
        InitMainY[counter] = 0;
    }
    InitCanvas();
    AddSysCurve();
    AddMainCurve();
}

OverallPlot::~OverallPlot()
{
    delete MainCurve;
    delete []SysCurve;
    delete []Limits;
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
    OverallGrid->setMinorPen(QPen(Qt::gray,  0, Qt::DotLine));
    OverallGrid->attach(this);
    //Set Axes
    setAxisScale(QwtPlot::yLeft  , 0.0, OverallYIniMax);
    setAxisScale(QwtPlot::xBottom, 0.0, ChannelNum - 1);
    //setAutoReplot(true);
}

void OverallPlot::AddMainCurve()
{
    MainCurve = new QwtPlotCurve;
    //Set curve color
    MainCurve->setPen(Qt::yellow, 2);
    //Set sample dot color
    QwtSymbol *SymbolBuf =
            new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::yellow),
                          QPen(Qt::red, 2), QSize(6, 6));
    MainCurve->setSymbol(SymbolBuf);
    MainCurve->setSamples(InitMainX, InitMainY, ChannelNum);
    MainCurve->attach(this);
}

void OverallPlot::AddSysCurve()
{
    SysCurve = new QwtPlotCurve[2];
    SysCurve[0].setPen(Qt::green,2);
    SysCurve[1].setPen(Qt::red,  2);
    SysCurve[0].setSamples(SysCurveXmin, SysCurveY, 2);
    SysCurve[1].setSamples(SysCurveXmax, SysCurveY, 2);
    SysCurve[0].attach(this);
    SysCurve[1].attach(this);
}

