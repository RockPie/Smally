#include "gpaintingsystem.h"

GPlot::GPlot(QWidget* parent):
    QWidget(parent)
{
    parentWidget = parent;

    Data = new QVector <QPointF>;

    CurvePen.setColor(Qt::yellow);
    CurvePen.setWidth(3);
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

    //Adjust Axis Scale
    if(isAutoScale)
    {
        double MaxYVal = Data->value(0).y();
        for(int counter = 1; counter < Data->length(); counter++)
            if(MaxYVal < Data->value(counter).y())
                MaxYVal = Data->value(counter).y();

        if(MaxYVal > Threshold * this->AxisInterval(yLeft).maxValue)
            setAxisScale(yLeft, this->AxisInterval(yLeft).minValue,
                                this->AxisInterval(yLeft).maxValue * 1.5);
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
                    int((x_buff - xMin) / (xMax - xMin) * this->width()),
                    int((yMax - y_buff) / (yMax - yMin) * this->height()));
            }
        }
    }
    else
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
                    int((x_buff[0] - xMin) / (xMax - xMin) * this->width()),
                    int((yMax - y_buff[0]) / (yMax - yMin) * this->height()),
                    int((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                    int((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
            }
        }
    }

    //Paint System Curve
    if(isSysEnabled)
    {
        painter.setPen(SysMinPen);
        painter.drawLine(
            int((SysMin - xMin) / (xMax - xMin) * this->width()), 0,
            int((SysMin - xMin) / (xMax - xMin) * this->width()), this->height());
        painter.setPen(SysMaxPen);
        painter.drawLine(
            int((SysMax - xMin) / (xMax - xMin) * this->width()), 0,
            int((SysMax - xMin) / (xMax - xMin) * this->width()), this->height());
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
    if(val <= xMax && val >= xMin && val <= SysMax)
        SysMin = val;
    this->replot();
    emit SysMinchanged(val);
}

void GPlot::setSysMax(double val)
{
    if(val <= xMax && val >= xMin && val >= SysMin)
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


