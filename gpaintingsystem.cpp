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
    SysMinPen.setWidth(3);
    SysMinPen.setStyle(Qt::SolidLine);

    SysMaxPen.setColor(Qt::red);
    SysMaxPen.setWidth(3);
    SysMaxPen.setStyle(Qt::SolidLine);

    RubberPen.setColor(QColor(0,180,180));
    RubberPen.setWidth(2);
    RubberPen.setStyle(Qt::DashLine);

    InfoBoxPen.setColor(QColor(0,180,180));
    InfoBoxPen.setWidth(2);
    InfoBoxPen.setStyle(Qt::SolidLine);

    InfoPen.setColor(QColor(0,180,180));
    InfoPen.setWidth(1);

    MidLinePen.setColor(QColor(255,117,117));
    MidLinePen.setWidth(2);
    MidLinePen.setStyle(Qt::DashDotLine);


    InfoFont = QFont("Arial", 11, QFont::Bold, true);
    InfoFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    QColor BackgroundColor = qRgb(52, 48, 41);
    QPalette Background(this->palette());


    Background.setColor(QPalette::Background, BackgroundColor);
    this->setAutoFillBackground(true);
    this->setPalette(Background);

    this->setAxisScale(yLeft, -10, 10);
    this->setAxisScale(xBottom, 0, ChannelNum);

    this->setMouseTracking(true);

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

    int Nearest_pixel_x = 0, Nearest_pixel_y = 0;
    double Nearest_origin_x = 0, Nearest_origin_y = 0;
    double Nearest_distance = ChannelNum * ChannelNum;
    double distancebuffer;

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
    /***    Dots Painting   ***/
    if(isDots)
    {
        double x_buff = 0, y_buff = 0;
        int pixelbuff_x = 0, pixelbuff_y = 0;
        for(int counter = 0; counter < Data->length(); counter++)
        {
            x_buff = double(Data->value(counter).x());
            y_buff = double(Data->value(counter).y());
            if((x_buff <= xMax && x_buff >= xMin) &&
               (y_buff <= yMax && y_buff >= yMin))
            {
                pixelbuff_x = qRound((x_buff - xMin) / (xMax - xMin) * this->width());
                pixelbuff_y = qRound((yMax - y_buff) / (yMax - yMin) * this->height());
                distancebuffer = pow((pixelbuff_x - xLoc), 2)+pow((pixelbuff_y - yLoc), 2);
                if(Nearest_distance > distancebuffer)
                {
                    Nearest_distance = distancebuffer;
                    Nearest_pixel_x = pixelbuff_x;
                    Nearest_pixel_y = pixelbuff_y;
                    Nearest_origin_x = x_buff;
                    Nearest_origin_y = y_buff;
                }
                painter.drawPoint(pixelbuff_x, pixelbuff_y);
            }
        }
    }
    else
    {
        /***    Line Painting   ***/
        if     (LineFilter == NoFilter)
        {
            double x_buff[2] = {0}, y_buff[2] = {0};
            int pixelbuff_x = 0, pixelbuff_y = 0;
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
                    pixelbuff_x = qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width());
                    pixelbuff_y = qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height());
                    distancebuffer = pow((pixelbuff_x - xLoc), 2)+pow((pixelbuff_y - yLoc), 2);
                    if(Nearest_distance > distancebuffer)
                    {
                        Nearest_distance = distancebuffer;
                        Nearest_pixel_x = pixelbuff_x;
                        Nearest_pixel_y = pixelbuff_y;
                        Nearest_origin_x = x_buff[0];
                        Nearest_origin_y = y_buff[0];
                    }
                    painter.drawLine(pixelbuff_x, pixelbuff_y,
                        qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
                }
            }
            pixelbuff_x = qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width());
            pixelbuff_y = qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height());
            distancebuffer = pow((pixelbuff_x - xLoc), 2)+pow((pixelbuff_y - yLoc), 2);
            if(Nearest_distance > distancebuffer)
            {
                Nearest_distance = distancebuffer;
                Nearest_pixel_x = pixelbuff_x;
                Nearest_pixel_y = pixelbuff_y;
                Nearest_origin_x = x_buff[1];
                Nearest_origin_y = y_buff[1];
            }
        }
        /***    3-Points Filter Painting   ***/
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
        /***    5-Points Filter Painting   ***/
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
        /***    High Accuracy Mode Painting   ***/
        else if(LineFilter == HighAccuracy)
        {
            double x_buff[2] = {0}, y_buff[2] = {0};
            int pixelbuff_x = 0, pixelbuff_y = 0;
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
                    pixelbuff_x = qRound((x_buff[0] - xMin) / (xMax - xMin) * this->width());
                    pixelbuff_y = qRound((yMax - y_buff[0]) / (yMax - yMin) * this->height());
                    distancebuffer = pow((pixelbuff_x - xLoc), 2)+pow((pixelbuff_y - yLoc), 2);
                    if(Nearest_distance > distancebuffer)
                    {
                        Nearest_distance = distancebuffer;
                        Nearest_pixel_x = pixelbuff_x;
                        Nearest_pixel_y = pixelbuff_y;
                        Nearest_origin_x = x_buff[0];
                        Nearest_origin_y = y_buff[0];
                    }
                    painter.drawLine(pixelbuff_x, pixelbuff_y,
                        qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width()),
                        qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height()));
                }
            }
            pixelbuff_x = qRound((x_buff[1] - xMin) / (xMax - xMin) * this->width());
            pixelbuff_y = qRound((yMax - y_buff[1]) / (yMax - yMin) * this->height());
            distancebuffer = pow((pixelbuff_x - xLoc), 2)+pow((pixelbuff_y - yLoc), 2);
            if(Nearest_distance > distancebuffer)
            {
                Nearest_distance = distancebuffer;
                Nearest_pixel_x = pixelbuff_x;
                Nearest_pixel_y = pixelbuff_y;
                Nearest_origin_x = x_buff[1];
                Nearest_origin_y = y_buff[1];
            }
        }


    }

    if(drawRubberBand && (isDots || LineFilter == NoFilter || LineFilter == HighAccuracy) && isMouseIn)
    {
        painter.setPen(RubberPen);
        painter.drawLine(Nearest_pixel_x, Nearest_pixel_y, xLoc, yLoc);
        QRect InfoBox;
        if(Nearest_pixel_x - 20 < xLoc && xLoc < this->width() - RecLength)
            if(yLoc < RecWidth)
                InfoBox = QRect(xLoc, yLoc, RecLength, RecWidth);
            else
                InfoBox = QRect(xLoc, yLoc - RecWidth, RecLength, RecWidth);
        else
            if(yLoc < RecWidth)
                InfoBox = QRect(xLoc - RecLength, yLoc, RecLength, RecWidth);
            else
                InfoBox = QRect(xLoc - RecLength, yLoc - RecWidth, RecLength, RecWidth);
        painter.setPen(InfoBoxPen);
        painter.drawRect(InfoBox);
        painter.setFont(InfoFont);
        painter.setPen(InfoPen);
        QString info = "道址:" + QString::number(Nearest_origin_x) + "\n计数:" +
                       QString::number(Nearest_origin_y);
        painter.drawText(InfoBox, Qt::AlignLeft, info);
    }

    /***    Paint System Curve  ***/
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

    if(isMidLine)
    {
        double sumRes = 0;
        for(int counter = 0; counter < Data->length(); counter++)
            sumRes += double(Data->value(counter).y());
        int aveRes = qRound((yMax - sumRes / ChannelNum) / (yMax - yMin) * this->height());
        painter.setPen(MidLinePen);
        painter.drawLine(0, aveRes, this->width(), aveRes);
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

void GPlot::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    xLoc = event->pos().x();
    yLoc = event->pos().y();
    if(isSysEnabled)
    {
        if(isDraggingMax)
        {
             int real_x = int((double(event->pos().x()) * (xMax - xMin) / this->width()) + xMin);
             if(abs(real_x - qRound(SysMax)) > 5)
                emit MouseSysMax(double(real_x));
        }
        else if(isDraggingMin)
        {
            int real_x = int((double(event->pos().x()) * (xMax - xMin) / this->width()) + xMin);
            if(abs(real_x - qRound(SysMin)) > 5)
                emit MouseSysMin(double(real_x));
        }
    }
}

void GPlot::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if(isSysEnabled)
        if(event->button() == Qt::LeftButton)
        {
            int real_x = int((double(event->pos().x()) * (xMax - xMin) / this->width()) + xMin);
            int MaxDistance = abs(real_x - qRound(SysMax));
            int MinDistance = abs(real_x - qRound(SysMin));
            if(MaxDistance < MinDistance && MaxDistance < 100)
            {
                isDraggingMax = true;
                isDraggingMin = false;
                emit MouseSysMax(double(real_x));
            }
            else if(MaxDistance >= MinDistance && MinDistance < 100)
            {
                isDraggingMax = false;
                isDraggingMin = true;
                emit MouseSysMin(double(real_x));
            }
        }
}

void GPlot::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    isDraggingMax = false;
    isDraggingMin = false;
}

void GPlot::enterEvent(QEvent *event)
{
    event->accept();
    QCursor crossCursor;
    crossCursor.setShape(Qt::CrossCursor);
    this->setCursor(crossCursor);
    isMouseIn = true;
}

void GPlot::leaveEvent(QEvent *event)
{
    event->accept();
    isMouseIn = false;
}

