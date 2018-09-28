#include "gplot.h"

OverallPlot::OverallPlot(QWidget *parent):
    QwtPlot(parent)
{
    Limits = new uint64_t[2];
    Limits[0] = uint64_t(0);
    Limits[1] = uint64_t(OverallYIniMax);
    SysCurveY[0] = double(Limits[0]) - double(Limits[1]) * 1.1;
    SysCurveY[1] = 2.1 * double(Limits[1]);
    SysCurveXmin[0] = OAMinLinePos;
    SysCurveXmin[1] = OAMinLinePos;
    SysCurveXmax[0] = OAMaxLinePos;
    SysCurveXmax[1] = OAMaxLinePos;
    for(int counter = 0; counter < ChannelNum; counter++)
    {
        InitMainX[counter] = counter;
        InitMainY[counter] = 0;
    }
    OASlider = new DoubleSlider(parent);    //Creat double slider
    AttachedPlot = new PartPlot(parent);
    InitCanvas();
    AddSysCurve();
    AddMainCurve();
    connect(OASlider,           &DoubleSlider::maxValueChanged,
            this,               &OverallPlot::MaxSysChange);
    connect(OASlider,           &DoubleSlider::minValueChanged,
            this,               &OverallPlot::MinSysChange);
    qDebug()<<"Main Plot Initialized";
}

OverallPlot::~OverallPlot()
{
    delete MainCurve;
    delete []SysCurve;
    delete OASlider;
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
    //Set axis
    setAxisScale(QwtPlot::yLeft  , 0.0, OverallYIniMax);
    setAxisScale(QwtPlot::xBottom, 0.0, ChannelNum - 1);
    //Disable axis display
    enableAxis(QwtPlot::yLeft, false);
    enableAxis(QwtPlot::xBottom, false);
    //Set auto scale
    //setAxisAutoScale(QwtPlot::yLeft, true);
    setAutoReplot(true);
}

void OverallPlot::AddMainCurve()
{
    MainCurve = new QwtPlotCurve;
    //Set curve color
    MainCurve->setPen(Qt::yellow, 2);
    //Set sample dot color
    /*
    QwtSymbol *SymbolBuf =
            new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::yellow),
                          QPen(Qt::red, 2), QSize(2, 2));
    MainCurve->setSymbol(SymbolBuf);
    */
    MainCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    MainCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush);
    MainCurve->setStyle(QwtPlotCurve::Lines);
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
    SysCurve[0].setStyle(QwtPlotCurve::Lines);
    SysCurve[1].setStyle(QwtPlotCurve::Lines);
    SysCurve[0].attach(this);
    SysCurve[1].attach(this);
}

void OverallPlot::OADataReceive(
        const QVector <QPointF> OAdata)
{
    MainCurve->setSamples(OAdata);
    AttachedPlot->setPPData(OAdata);
    uint64_t MaxVal = uint64_t(OAdata[0].y());
    for(int counter = 1; counter < OAdata.length(); counter++)
        if(MaxVal < uint64_t(OAdata[counter].y()))
            MaxVal = uint64_t(OAdata[counter].y());
    if(MaxVal > uint64_t(0.9 * axisInterval(QwtPlot::yLeft).maxValue()))
    {
        SysCurveY[0] = double(MaxVal) * (-1.1);
        SysCurveY[1] = double(MaxVal) * 2.1;
        setAxisScale(QwtPlot::yLeft  , 0.0,
                     1.5 * axisInterval(QwtPlot::yLeft).maxValue());
    }
    SysCurveRefresh();
}

void OverallPlot::MinSysChange(double val)
{
    SysCurveXmin[0] = val;
    SysCurveXmin[1] = val;
    SysCurveRefresh();
    AttachedPlot->setAxisScale(QwtPlot::xBottom, val,
                               axisInterval(QwtPlot::yLeft).maxValue());
}

void OverallPlot::MaxSysChange(double val)
{
    SysCurveXmax[0] = val;
    SysCurveXmax[1] = val;
    SysCurveRefresh();
    AttachedPlot->setAxisScale(QwtPlot::xBottom,
                               axisInterval(QwtPlot::yLeft).minValue(), val);
}

void OverallPlot::setDotDisplay(bool isDot)
{
    if(isDot)
    {
        MainCurve->setStyle(QwtPlotCurve::Dots);
        AttachedPlot->setPPDots();
    }
    else
    {
        MainCurve->setStyle(QwtPlotCurve::Lines);
        AttachedPlot->setPPLines();
    }
}

void OverallPlot::setLogMode(bool isLog){}

PartPlot::PartPlot(QWidget *parent):
    QwtPlot(parent)
{
    for(int counter = 0; counter < ChannelNum; counter++)
    {
        InitPartX[counter] = counter;
        InitPartY[counter] = 0;
    }
    InitCanvas();
    AddPartCurve();
}

PartPlot::~PartPlot(){}

void PartPlot::InitCanvas()
{
    //Set background color
    this->canvas()->setPalette(QPalette(QColor(Qt::black)));
    setAutoFillBackground(true);
    //Set grid
    QwtPlotGrid  *PartGrid = new QwtPlotGrid;
    PartGrid->enableXMin(true);
    PartGrid->enableYMin(true);
    //Grid style
    PartGrid->setMajorPen(QPen(Qt::white, 0, Qt::DotLine));
    PartGrid->setMinorPen(QPen(Qt::gray,  0, Qt::DotLine));
    PartGrid->attach(this);
    //Set axis
    setAxisScale(QwtPlot::yLeft  , 0.0, OverallYIniMax);
    setAxisScale(QwtPlot::xBottom, 0.0, ChannelNum - 1);
    //Disable axis display
    enableAxis(QwtPlot::yLeft, false);
    enableAxis(QwtPlot::xBottom, false);
    //Set auto scale
    setAxisAutoScale(QwtPlot::yLeft, true);
    setAutoReplot(true);
}

void PartPlot::AddPartCurve()
{
    PartCurve = new QwtPlotCurve;
    //Set curve color
    PartCurve->setPen(Qt::yellow, 2);
    //Set sample dot color
    /*
    QwtSymbol *SymbolBuf =
            new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::yellow),
                          QPen(Qt::red, 2), QSize(2, 2));
    MainCurve->setSymbol(SymbolBuf);
    */
    PartCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    PartCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush);
    PartCurve->setStyle(QwtPlotCurve::Lines);
    PartCurve->setSamples(InitPartX, InitPartY, ChannelNum);
    PartCurve->attach(this);
}
