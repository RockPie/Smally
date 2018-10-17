#ifndef GPAINTINGSYSTEM_H
#define GPAINTINGSYSTEM_H

#include <QtGui>
#include <QPainter>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QPalette>
#include <QVector>
#include "gsetting.h"

enum AxisType{
  yLeft = 0, yRight, xBottom, xTop
};

enum FilterType{
    NoFilter = 0, Three_Points, Five_Points, HighAccuracy
};

struct Border{
    double maxValue;
    double minValue;
};

class GPlot: public QWidget
{
    Q_OBJECT
public:
    GPlot(QWidget* parent);
    ~GPlot();
    void setAxisScale(AxisType Axis, double min, double max);

    inline void setSamples(QVector <QPointF> dataSeries);
    Border AxisInterval(AxisType Axis) const;
    inline void replot();
    inline void setLinePattern(bool input);
    inline void enableSysCurve();
    inline void disableSysCurve();
    inline double getSysMax() const;
    inline double getSysMin() const;
    inline void setStyle(Qt::PenStyle input);
    inline void enableAutoScale(bool isenable = true);
    inline void disableAutoScale(bool isdisable = true);

public slots:
    void setSysMin(double val);
    void setSysMax(double val);
    void setMaxBorder(double val);
    void setMinBorder(double val);
    void setFilter(FilterType type);

signals:
    void SysMaxchanged(double val);
    void SysMinchanged(double val);
    void MaxCountChanged(QString val);
    void MinCountChanged(QString val);

private:
    void paintEvent(QPaintEvent *event);
    void updateBackground(QPainter *BGpainter);

private:
    QVector <QPointF>* Data;
    QWidget* parentWidget;
    QPen CurvePen, SysMinPen, SysMaxPen, AxisPen;
    bool isDotDisplay = false;
    double xMin = 0, xMax = 0;
    double yMin = 0, yMax = 0;

    double SysMin = 0, SysMax = 0;

    bool isDots = false;
    bool isSysEnabled = false;
    bool isAutoScale = false;
    FilterType LineFilter = NoFilter;

};

inline void GPlot::setSamples(QVector <QPointF> dataSeries){
    Data->swap(dataSeries);
}

inline void GPlot::replot(){
    this->update();
}

inline void GPlot::setLinePattern(bool input){
    isDots = input;
}

inline void GPlot::enableSysCurve(){
    isSysEnabled = true;
}

inline void GPlot::disableSysCurve(){
    isSysEnabled = false;
}

inline double GPlot::getSysMax() const{
    return SysMax;
}

inline double GPlot::getSysMin() const{
    return SysMin;
}

inline void GPlot::setStyle(Qt::PenStyle input){
    CurvePen.setStyle(input);
}

inline void GPlot::enableAutoScale(bool isenable){
    isAutoScale = isenable;
}

inline void GPlot::disableAutoScale(bool isdisable){
    isAutoScale = ~isdisable;
}

#endif // GPAINTINGSYSTEM_H
