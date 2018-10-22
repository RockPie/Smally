#include "gpaintingsystem.h"

GPlot::GPlot(QWidget* parent):
    QWidget(parent)
{
    parentWidget = parent;

    Data = new QVector <QPointF>;

    CurvePen.setColor(Qt::yellow);
    CurvePen.setWidth(2);
    CurvePen.setStyle(Qt::SolidLine);

    QColor AxisColor = qRgb(100,100,100);
    AxisPen.setColor(AxisColor);
    AxisPen.setWidth(2);
    AxisPen.setStyle(Qt::DashLine);

    SysMinPen.setColor(Qt::green);
    SysMinPen.setWidth(2);
    SysMinPen.setStyle(Qt::SolidLine);

    SysMaxPen.setColor(Qt::red);
    SysMaxPen.setWidth(2);
    SysMaxPen.setStyle(Qt::SolidLine);

    QColor BackgroundColor = qRgb(52, 48, 41);
    QPalette Background(this->palette());


    Background.setColor(QPalette::Background, BackgroundColor);
    this->setAutoFillBackground(true);
    this->setPalette(Background);

    this->setAxisScale(yLeft, -10, 10);
    this->setAxisScale(xBottom, 0, ChannelNum);


}

GPlot::~GPlot()
{
    delete Data;
}

void GPlot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    double MaxYVal = Data->value(0).y();
    double MinYVal = Data->value(0).y();

    for(int counter = 1; counter < Data->length(); counter++)
    {
        if(MaxYVal < Data->value(counter).y())
            MaxYVal = Data->value(counter).y();
        else if(MinYVal > Data->value(counter).y())
            MinYVal = Data->value(counter).y();
    }

    emit MaxCountChanged(QString::number(MaxYVal, 10,2));
    emit MinCountChanged(QString::number(MinYVal, 10,2));

    //Adjust Axis Scale
    if(isAutoScale)
    {
        if(MaxYVal > ScaleThresholdH * this->AxisInterval(yLeft).maxValue)
            setAxisScale(yLeft, this->AxisInterval(yLeft).minValue,
                                MaxYVal * 1.5);
        if(MaxYVal < this->AxisInterval(yLeft).maxValue * ScaleThresholdL)
            setAxisScale(yLeft, this->AxisInterval(yLeft).minValue,
                                MaxYVal * 1.5);
    }

    this->updateBackground(&painter);

    //Paint Data Curve
    painter.setPen(CurvePen);
    if(isDots)
    {
        double x_buff = 0, y_buff = 0;
        for(int counter = 0; counter < Data->length(); counter++)
        {
            x_buff = double(Data->value(counter).x());
            y_buff = double(Data->value(counter).y());
            if((x_buff <= xMax && x_buff >= xMin) &&
               (y_buff <= yMax && y_buff >= yMin))
            {
                painter.drawPoint(
                    qRound((x_buff - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff) / (yMax - yMin) * this->height()));
            }
        }
    }
    else
    {
        if     (LineFilter == NoFilter)
        {
            double x_buff[2] = {0}, y_buff[2] = {0};
            x_buff[1] = double(Data->value(0).x());
            y_buff[1] = double(Data->value(0).y());
            for(int counter = 1; counter < Data->length(); counter++)
            {
                x_buff[0] = x_buff[1];
                x_buff[1] = double(Data->value(counter).x());
                y_buff[0] = y_buff[1];
                y_buff[1] = double(Data->value(counter).y());
                if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
                   (y_buff[1] <= yMax && y_buff[1] >= yMin))
                {
                    if(counter == 1)
                        if((x_buff[0] <= xMax && x_buff[0] >= xMin) &&
                           (y_buff[0] <= yMax && y_buff[0] >= yMin) == false)
                            continue;
                    painter.drawLine(
                        qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                        qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
                }
            }
        }
        else if(LineFilter == Three_Points)
        {
            double x_buff[2] = {0}, y_buff[5] = {0};
            x_buff[1] = double(Data->value(0).x());
            y_buff[1] = double(Data->value(0).y());
            for(int counter = 1; counter < Data->length() - 1; counter++)
            {
                x_buff[0] = x_buff[1];
                x_buff[1] = double(Data->value(counter).x());
                y_buff[0] = y_buff[1];
                y_buff[2] = double(Data->value(counter - 1).y());
                y_buff[3] = double(Data->value(counter).y());
                y_buff[4] = double(Data->value(counter + 1).y());
                y_buff[1] = (y_buff[4] + y_buff[3] + y_buff[2]) / 3.0;
                if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
                   (y_buff[1] <= yMax && y_buff[1] >= yMin))
                {
                    if(counter == 1)
                        if((x_buff[0] <= xMax && x_buff[0] >= xMin) &&
                          (y_buff[0] <= yMax && y_buff[0] >= yMin) == false)
                            continue;
                    painter.drawLine(
                        qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                        qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
                }
            }
            x_buff[0] = x_buff[1];
            x_buff[1] = double(Data->value(Data->length() - 1).x());
            y_buff[0] = y_buff[1];
            y_buff[1] = double(Data->value(Data->length() - 1).y());
            if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
               (y_buff[4] <= yMax && y_buff[4] >= yMin))
                painter.drawLine(
                    qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                    qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
        }
        else if(LineFilter == Five_Points)
        {
            double x_buff[2] = {0}, y_buff[7] = {0};
            x_buff[0] = double(Data->value(0).x());
            x_buff[1] = double(Data->value(1).x());
            y_buff[0] = double(Data->value(0).y());
            y_buff[1] = double(Data->value(1).y());
            if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
              (y_buff[1] <= yMax && y_buff[1] >= yMin))
            {
                if((x_buff[0] <= xMax && x_buff[0] >= xMin) &&
                  (y_buff[0] <= yMax && y_buff[0] >= yMin) == true)
                    painter.drawLine(
                            qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                            qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                            qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                            qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
            }
            for(int counter = 2; counter < Data->length() - 2; counter++)
            {
                x_buff[0] = x_buff[1];
                x_buff[1] = double(Data->value(counter).x());
                y_buff[0] = y_buff[1];
                y_buff[2] = double(Data->value(counter - 2).y());
                y_buff[3] = double(Data->value(counter - 1).y());
                y_buff[4] = double(Data->value(counter).y());
                y_buff[5] = double(Data->value(counter + 1).y());
                y_buff[6] = double(Data->value(counter + 2).y());
                y_buff[1] = (y_buff[6] + y_buff[5] + y_buff[4] + y_buff[3] + y_buff[2]) / 5.0;
                if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
                   (y_buff[1] <= yMax && y_buff[1] >= yMin))
                    painter.drawLine(
                        qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                        qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
            }
            x_buff[0] = x_buff[1];
            x_buff[1] = double(Data->value(Data->length() - 2).x());
            y_buff[0] = y_buff[1];
            y_buff[1] = double(Data->value(Data->length() - 2).y());
            if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
               (y_buff[4] <= yMax && y_buff[4] >= yMin))
                painter.drawLine(
                    qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                    qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
            x_buff[0] = x_buff[1];
            x_buff[1] = double(Data->value(Data->length() - 1).x());
            y_buff[0] = y_buff[1];
            y_buff[1] = double(Data->value(Data->length() - 1).y());
            if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
               (y_buff[4] <= yMax && y_buff[4] >= yMin))
                painter.drawLine(
                    qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                    qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                    qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
        }
        else if(LineFilter == HighAccuracy)
        {
            double x_buff[2] = {0}, y_buff[2] = {0};
            x_buff[1] = double(Data->value(0).x());
            y_buff[1] = double(Data->value(0).y());
            for(int counter = 1; counter < Data->length(); counter++)
            {
                x_buff[0] = x_buff[1];
                x_buff[1] = double(Data->value(counter).x());
                y_buff[0] = y_buff[1];
                y_buff[1] = double(Data->value(counter).y());
                if((x_buff[1] <= xMax && x_buff[1] >= xMin) &&
                   (y_buff[1] <= yMax && y_buff[1] >= yMin))
                {
                    if(counter == 1)
                        if((x_buff[0] <= xMax && x_buff[0] >= xMin) &&
                           (y_buff[0] <= yMax && y_buff[0] >= yMin) == false)
                            continue;
                    painter.drawLine(
                        qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                        qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
                }
            }
        }
    }

    //Paint System Curve
    if(isSysEnabled)
    {
        painter.setPen(SysMinPen);
        painter.drawLine(
            qRound((SysMin - xMin) / (xMax - xMin) * this->width()), 0,
            qRound((SysMin - xMin) / (xMax - xMin) * this->width()), this->height());
        painter.setPen(SysMaxPen);
        painter.drawLine(
            qRound((SysMax - xMin) / (xMax - xMin) * this->width()), 0,
            qRound((SysMax - xMin) / (xMax - xMin) * this->width()), this->height());
    }

}

void GPlot::updateBackground(QPainter *BGpainter)
{
    //Should be disabled if high accuracy is needed
    //BGpainter.setRenderHint(QPainter::Antialiasing, true);
    QSize WidgetSize = this->size();
    int xInterval = WidgetSize.width()  / GridNum;
    int yInterval = WidgetSize.height() / GridNum;
    BGpainter->setPen(AxisPen);
    for(int counter = 0; counter < GridNum; counter++)
        BGpainter->drawLine(
                    0, WidgetSize.height() - yInterval * counter,
                    WidgetSize.width(), WidgetSize.height() - yInterval * counter);
    for(int counter = 1; counter < GridNum; counter++)
        BGpainter->drawLine(
                    xInterval * counter, 0,
                    xInterval * counter, WidgetSize.height());
}

void GPlot::setAxisScale(AxisType Axis, double min, double max)
{
    if(Axis == yLeft || Axis == yRight)
    {
        yMin = min;
        yMax = max;
    }
    else
    {
        xMin = min;
        xMax = max;
    }
}

Border GPlot::AxisInterval(AxisType Axis) const
{
    Border res;
    if(Axis == yLeft)
    {
        res.maxValue = yMax;
        res.minValue = yMin;
    }
    else if(Axis == xBottom)
    {
        res.maxValue = xMax;
        res.minValue = xMin;
    }
    return res;
}

void GPlot::setSysMin(double val)
{
    //if(val <= xMax && val >= xMin && val <= SysMax)
        SysMin = val;
    this->replot();
    emit SysMinchanged(val);
}

void GPlot::setSysMax(double val)
{
   // if(val <= xMax && val >= xMin && val >= SysMin)
        SysMax = val;
    this->replot();
    emit SysMaxchanged(val);
}

void GPlot::setMaxBorder(double val)
{
    if(AxisInterval(xBottom).minValue < val)
        this->setAxisScale(xBottom, AxisInterval(xBottom).minValue, val);
}

void GPlot::setMinBorder(double val)
{
    if(AxisInterval(xBottom).maxValue > val)
        this->setAxisScale(xBottom, val, AxisInterval(xBottom).maxValue);
}

void GPlot::setFilter(FilterType type){
    LineFilter = type;
}

