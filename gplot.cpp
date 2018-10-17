#include "gplot.h"
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
    OASlider->setLabel("HaHaHa");
    connect(OASlider,       &DoubleSlider::maxValueChanged,
            this,           &GPlot::setSysMax);
    connect(OASlider,       &DoubleSlider::minValueChanged,
            this,           &GPlot::setSysMin);
    connect(this,           &GPlot::SysMaxchanged,
            AttachedPlot,   &GPlot::setMaxBorder);
    connect(this,           &GPlot::SysMinchanged,
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

void OverallPlot::setXLogMode(bool isLog)
{
    if(isLog)
    {
        setAxisScale(xBottom, 0.0, LogChannel);
        setSysMin(double(20) * qLn(this->getSysMin() + 1));
        setSysMax(double(20) * qLn(this->getSysMax() + 1));
        //The order of setting OASlider is important
        OASlider->setSingleStep(0.01);
        OASlider->setMinValue(this->getSysMin());
        OASlider->setMaxValue(this->getSysMax());

        OASlider->setRange(0, LogChannel);
    }
    else
    {
        setAxisScale(xBottom, 0.0, ChannelNum - 1);
        setSysMin(double(qRound((exp(0.05 * this->getSysMin())) - 1)));
        setSysMax(double(qRound((exp(0.05 * this->getSysMax())) - 1)));
        OASlider->setSingleStep(1.0);
        OASlider->setMaxValue(this->getSysMax());
        OASlider->setMinValue(this->getSysMin());
        OASlider->setRange(0, ChannelNum - 1);
    }
    this->replot();
    AttachedPlot->replot();
}

void OverallPlot::setYLogMode(bool isLog)
{
    if(isLog)
    {
        setAxisScale(yLeft, 0.0,
                     double(20) * (qLn(this->AxisInterval(yLeft).maxValue + 1)));
        AttachedPlot->setAxisScale(yLeft, 0.0,
            double(20) * (qLn(AttachedPlot->AxisInterval(yLeft).maxValue + 1)));
    }
    else
    {
        setAxisScale(yLeft, 0.0,
                     double(qRound((exp(0.05 * this->AxisInterval(yLeft).maxValue)) - 1)));
        AttachedPlot->setAxisScale(yLeft, 0.0,
            double(qRound((exp(0.05 * AttachedPlot->AxisInterval(yLeft).maxValue)) - 1)));
    }
    this->replot();
    AttachedPlot->replot();
}

void OverallPlot::OADataReceive(
        const QVector<QPointF> OAdata)
{
    this->setSamples(OAdata);
    AttachedPlot->setSamples(OAdata);
    this->replot();
    AttachedPlot->replot();
}

void OverallPlot::setAutoScale(bool isAuto){
    this->enableAutoScale(isAuto);
    AttachedPlot->enableAutoScale(isAuto);
}

void OverallPlot::setThreePoints(bool isTPF)
{
    if(isTPF)
    {
        this->setFilter(Three_Points);
        AttachedPlot->setFilter(Three_Points);
    }
    else
    {
        this->setFilter(NoFilter);
        AttachedPlot->setFilter(NoFilter);
    }
    this->replot();
    AttachedPlot->replot();
}

void OverallPlot::setFivePoints(bool isFPF)
{
    if(isFPF)
    {
        this->setFilter(Five_Points);
        AttachedPlot->setFilter(Five_Points);
    }
    else
    {
        this->setFilter(NoFilter);
        AttachedPlot->setFilter(NoFilter);
    }
    this->replot();
    AttachedPlot->replot();
}

void OverallPlot::setHighAccuracy(bool isHAF)
{
    if(isHAF)
    {
        this->setFilter(HighAccuracy);
        AttachedPlot->setFilter(HighAccuracy);
    }
    else
    {
        this->setFilter(NoFilter);
        AttachedPlot->setFilter(NoFilter);
    }
    this->replot();
    AttachedPlot->replot();
}
