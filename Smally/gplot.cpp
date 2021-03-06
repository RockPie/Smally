#include "gplot.h"
/*
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
                               AttachedPlot->axisInterval(xBottom).maxValue());
}

void OverallPlot::MaxSysChange(double val)
{
    //qDebug()<<SysCurveXmax[0]<<"MaxChange1";
    SysCurveXmax[0] = val;
    //qDebug()<<SysCurveXmax[0]<<"MaxChange2";
    SysCurveXmax[1] = val;
    SysCurveRefresh();
    AttachedPlot->setAxisScale(QwtPlot::xBottom,
                               AttachedPlot->axisInterval(QwtPlot::xBottom).minValue(), val);
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

void OverallPlot::setXLogMode(bool isLog)
{
    if(isLog)
    {
        setAxisScale(QwtPlot::xBottom, 0.0, LogChannel);
        MinSysChange(double(20) * qLn(SysCurveXmin[0] + 1));
        MaxSysChange(double(20) * qLn(SysCurveXmax[0] + 1));
        //The order of setting OASlider is important
        OASlider->setSingleStep(0.01);
        OASlider->setMinValue(SysCurveXmin[0]);
        OASlider->setMaxValue(SysCurveXmax[0]);
        OASlider->setRange(0, LogChannel);
    }
    else
    {
        setAxisScale(QwtPlot::xBottom, 0.0, ChannelNum - 1);
        MinSysChange(double(qRound((exp(0.05 * SysCurveXmin[0])) - 1)));
        MaxSysChange(double(qRound((exp(0.05 * SysCurveXmax[0])) - 1)));
        OASlider->setSingleStep(1.0);
        OASlider->setMinValue(SysCurveXmin[0]);
        OASlider->setMaxValue(SysCurveXmax[0]);
        OASlider->setRange(0, ChannelNum - 1);
    }
    SysCurveRefresh();
}

void OverallPlot::setYLogMode(bool isLog)
{
    if(isLog)
        setAxisScale(QwtPlot::yLeft, 0.0,
                     double(20) * (qLn(axisInterval(QwtPlot::yLeft)
                                      .maxValue() + 1)));
    else
        setAxisScale(QwtPlot::yLeft, 0.0,
                     double(qRound((exp(0.05 * axisInterval(QwtPlot::yLeft)
                                        .maxValue())) - 1)));
}
*/

PartPlot::PartPlot(QWidget *parent):
    GPlot (parent)
{
    for(int counter = 0; counter < ChannelNum; counter++)
    {
        InitPartX[counter] = counter;
        InitPartY[counter] = 0;
    }
    this->setAxisScale(xBottom, 0.0, ChannelNum - 1);
    this->setAxisScale(yLeft,   0.0, OverallYIniMax);
    this->disableSysCurve();
    this->enableAutoScale();
    //InitCanvas();
    //AddPartCurve();
}

PartPlot::~PartPlot(){}

OverallPlot::OverallPlot(QWidget *parent):
    GPlot (parent)
{
    AttachedPlot = new PartPlot(parent);
    OASlider = new DoubleSlider(parent);
    this->enableSysCurve();
    setSysMax(ChannelNum - 1);
    setSysMin(0);
    this->enableAutoScale();
    this->setAxisScale(xBottom, 0.0, ChannelNum - 1);
    this->setAxisScale(yLeft,   0.0, OverallYIniMax);
    connect(OASlider,       &DoubleSlider::maxValueChanged,
            this,           &GPlot::setSysMax);
    connect(OASlider,       &DoubleSlider::minValueChanged,
            this,           &GPlot::setSysMin);
    connect(OASlider,       &DoubleSlider::maxValueChanged,
            AttachedPlot,   &GPlot::setMaxBorder);
    connect(OASlider,       &DoubleSlider::minValueChanged,
            AttachedPlot,   &GPlot::setMinBorder);
}

OverallPlot::~OverallPlot()
{
    delete OASlider;
    delete AttachedPlot;
}

void OverallPlot::setDotDisplay(bool val){
    this->setLinePattern(val);
    AttachedPlot->setLinePattern(val);
    this->replot();
    AttachedPlot->replot();
}

void OverallPlot::setXLogMode(bool val){}
void OverallPlot::setYLogMode(bool val){}

void OverallPlot::OADataReceive(
        const QVector<QPointF> OAdata)
{
    this->setSamples(OAdata);
    AttachedPlot->setSamples(OAdata);
    this->replot();
    AttachedPlot->replot();
}

/*
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
*/

/*
void PartPlot::AddPartCurve()
{
    PartCurve = new QwtPlotCurve;
    //Set curve color
    PartCurve->setPen(Qt::yellow, 2);
    //Set sample dot color
    PartCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    PartCurve->setLegendAttribute(QwtPlotCurve::LegendShowBrush);
    PartCurve->setStyle(QwtPlotCurve::Lines);
    PartCurve->setSamples(InitPartX, InitPartY, ChannelNum);
    PartCurve->attach(this);
}
*/
