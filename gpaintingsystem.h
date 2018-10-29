#ifndef GPAINTINGSYSTEM_H
#define GPAINTINGSYSTEM_H

#include <QtGui>
#include <QPainter>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QPalette>
#include <QVector>
#include <cmath>
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
    inline void setRubberBand(bool isband);
    inline void setMidLine(bool isMid);

public slots:
    void setSysMin(double val);
    void setSysMax(double val);
    void setMaxBorder(double val);
    void setMinBorder(double val);
    void setFilter(FilterType type);
    void updateMark();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void SysMaxchanged(double val);
    void SysMinchanged(double val);
    void MaxCountChanged(QString val);
    void MinCountChanged(QString val);
    void MouseSysMax(double val);
    void MouseSysMin(double val);


private:
    void paintEvent(QPaintEvent *event);
    void updateBackground(QPainter *BGpainter);

private:
    QVector <QPointF>* Data;
    QWidget* parentWidget;
    QPen CurvePen, SysMinPen, SysMaxPen, AxisPen, RubberPen,
         InfoBoxPen, InfoPen, MidLinePen;
    QBrush InfoBoxBrush;
    QFont InfoFont;
    bool isDotDisplay = false;
    double xMin = 0, xMax = 0;
    double yMin = 0, yMax = 0;
    int xLoc = 0, yLoc = 0;
    int CrossMarkLoc = 0;
    int delayCount = 10;


    double SysMin = 0, SysMax = 0;

    bool isDots = false;
    bool isSysEnabled = false;
    bool isAutoScale = false;
    bool drawRubberBand = false;
    bool isMidLine = false;
    bool isMouseIn = false;
    bool isMarkIncreasing = false;
    bool isMarkDecreasing = false;

    bool isDraggingMin = false;
    bool isDraggingMax = false;

    int Nearest_pixel_x = 0, Nearest_pixel_y = 0;
    double Nearest_origin_x = 0, Nearest_origin_y = 0;
    int Nearest_Num = 0;
    double Nearest_distance = ChannelNum * ChannelNum;
    double distancebuffer;

    bool isRubber = true;
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

inline void GPlot::setRubberBand(bool isband){
    drawRubberBand = isband;
}

inline void GPlot::setMidLine(bool isMid){
    isMidLine = isMid;
}
#endif // GPAINTINGSYSTEM_H
