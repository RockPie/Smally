#include "gcomponent.h"

DoubleSlider::DoubleSlider(QWidget* parent)
    : QWidget(parent)
    , m_min(double(0))
    , m_max(double(ChannelNum - 1))
    , m_singleStep(double(1.0))
    , m_minValue(double(0))
    , m_maxValue(double(ChannelNum - 1))
    , m_state(None)
{
    setFixedHeight(40);
    setFocusPolicy(Qt::StrongFocus);
}

DoubleSlider::~DoubleSlider(){}

void DoubleSlider::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    paintValueLabel(&painter);
}

void DoubleSlider::paintValueLabel(QPainter* painter)
{
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 11));

    QString minValueString = QString::number(m_minValue,'f',2);
    QString maxValueString = QString::number(m_maxValue,'f',2);

    QFontMetrics metrics = painter->fontMetrics();
    int mintextWidth = metrics.width(minValueString);
    int maxtextWidth = metrics.width(maxValueString);
    int textHeight = metrics.height();

    //paint text
    painter->drawText(QRectF(2,4,mintextWidth,textHeight),minValueString);
    painter->drawText(QRectF(width() - maxtextWidth -2, 4,
                             maxtextWidth,textHeight), maxValueString);

    //paint label
    painter->setFont(QFont("Arial",12));
    metrics = painter->fontMetrics();
    int labelWidth = metrics.width(m_label);
    int labelHeight = metrics.height();

    QRectF textRect = QRectF((width()/2 - labelWidth/2), 0,labelWidth,labelHeight);

    painter->drawText(textRect,m_label);


    int minPos = int(( m_minValue - m_min ) * width() / (m_max - m_min));
    int maxPos = int(( m_maxValue - m_min ) * width() / (m_max - m_min));

    if(minPos <= 4){
        minPos = 4;
    }else if(minPos >= width() - 8){
        minPos = width() - 8;
    }else{}

    if(maxPos <= 4){
        maxPos = 4;
    }else if(maxPos >= width() -8){
        maxPos = width() - 8;
    }else{}


    //----- paint groove
    paintColoredRect(QRect(4,37,width() - 8,2),Qt::gray,painter);
    paintColoredRect(QRect(minPos + 4,37,maxPos - minPos,2),
                     QColor(51,153,155),painter);

    //----- handle

    minHandleRegion = QRect(minPos ,30,8,16);
    maxHandleRegion = QRect(maxPos ,30,8,16);

    //-----paint Handle
    QColor minColor  = (m_state == MinHandle) ?
                QColor(34, 139, 34) : QColor(102, 205, 0);
    QColor maxColor  = (m_state == MaxHandle) ?
                QColor(178, 34, 34) : QColor(255, 0, 0);
    paintColoredRect(minHandleRegion,minColor,painter);
    paintColoredRect(maxHandleRegion,maxColor,painter);
}


void DoubleSlider::setRange(double min, double max)
{
    m_min = min;
    m_max = max;

    if(minValue() < m_min)
        setMinValue(m_min);

    if(maxValue() > m_max){
        setMaxValue(m_max);
    }
}

void DoubleSlider::setSingleStep(double step){
    m_singleStep = step;
}

void DoubleSlider::setMinValue(double val)
{
    if(fabs( m_minValue - val ) >= m_singleStep)
    {
        if(m_maxValue - val < m_singleStep)
            m_minValue = m_maxValue - m_singleStep;
        else if(int(m_singleStep) == 1)
            m_minValue = double(int(val));
        else
            m_minValue = val;
        emit minValueChanged(m_minValue);
    }
    update();
}

void DoubleSlider::setMaxValue(double val)
{
    if(fabs(m_maxValue - val) >= m_singleStep)
    {
        if(val - m_minValue < m_singleStep)
            m_maxValue = m_minValue + m_singleStep;
        else if(int(m_singleStep) == 1)
            m_maxValue = double(int(val));
        else
            m_maxValue = val;
        emit maxValueChanged(m_maxValue);
    }
    update();
}

void DoubleSlider::setLabel(const QString &label)
{
   m_label = label;
   update();
}

void DoubleSlider::leaveEvent(QEvent* event)
{
    QWidget::leaveEvent(event);
    m_state = None;
    update();
}

void DoubleSlider::mousePressEvent(QMouseEvent* event)
{
    if(minHandleRegion.contains(event->pos())){
        m_state = MinHandle;
    }else if(maxHandleRegion.contains(event->pos())){
        m_state = MaxHandle;
    }else{
        m_state = None;
    }

    if(fabs(minValue() - maxValue()) < m_singleStep){
        if(fabs(minValue() - m_min) < m_singleStep){
            m_state = MaxHandle;
        }else if(fabs(minValue() - m_max) < m_singleStep){
            m_state = MinHandle;
        }else{}
    }

    update();
}

void DoubleSlider::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton){
        double move = event->x() * (m_max - m_min) * 1.0/ width() + m_min;

        switch(m_state){
            case MinHandle:{
                    double val = getValidValue( move, m_min,maxValue());
                    setMinValue(val);
                    break;
            }
            case MaxHandle:{
                    double val = getValidValue(move, minValue(), m_max);
                    setMaxValue(val);
                    break;
                }
            case None:
                break;
        }
    }
    update();
}


void DoubleSlider::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);

    if(event->key() == Qt::Key_Left){
        if(m_state == MinHandle){
            double val = getValidValue(minValue()- m_singleStep, m_min ,maxValue());
            setMinValue(val);
        }else if(m_state == MaxHandle) {
            double val = getValidValue(maxValue()- m_singleStep, minValue() ,m_max);
            setMaxValue(val);
        }else{}
    }else if(event->key() == Qt::Key_Right){
        if(m_state == MinHandle){
            double val = getValidValue(minValue() + m_singleStep, m_min ,maxValue());
            setMinValue(val);
        }else if(m_state == MaxHandle) {
            double val = getValidValue(maxValue() + m_singleStep, minValue() ,m_max);
            setMaxValue(val);
        }else{}
    }

    update();

}

GGuideLight::GGuideLight(QWidget* parent):
    QWidget (parent)
{
    this->setMinimumSize(20, 20);
}

GGuideLight::~GGuideLight(){}

void GGuideLight::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.translate(width() / 2, height() / 2);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);
    painter.fillRect(-width(), -height(), width() * 2, height() * 2, Qt::gray);

    int radius = 8;

    //Outer Border
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    //Inner Border
    radius -= 2;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(166, 166, 166));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    //Color Inside
    radius -= 1;
    QRadialGradient rg(0, 0, radius);
    if(ColorType == Red)
    {
        rg.setColorAt(0, QColor(245, 0, 0));
        rg.setColorAt(0.6, QColor(210, 0, 0));
        rg.setColorAt(1, QColor(166, 0, 0));
    }
    else if(ColorType == Yellow)
    {
        rg.setColorAt(0, QColor(245, 245, 0));
        rg.setColorAt(0.6, QColor(210, 210, 0));
        rg.setColorAt(1, QColor(166, 166, 0));
    }
    else if(ColorType == Green)
    {
        rg.setColorAt(0, QColor(0, 245, 0));
        rg.setColorAt(0.6, QColor(0, 210, 0));
        rg.setColorAt(1, QColor(0, 166, 0));
    }
    painter.setBrush(rg);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    QPainterPath path;
    path.addEllipse(-radius, -radius, radius << 1, radius << 1);
    QPainterPath bigEllipse;

    radius *= 2;
    bigEllipse.addEllipse(-radius, -radius + 2, radius << 1, radius << 1);
    path -= bigEllipse;

    QLinearGradient lg3(0, -radius, 0, radius);
    lg3.setColorAt(0, QColor(255, 200, 200, 180));
    lg3.setColorAt(1, QColor(255, 200, 200, 100));
    painter.setBrush(lg3);
    painter.drawPath(path);
}

void GGuideLight::setColor(ButtonColor BCtype){
    ColorType = BCtype;
    this->update();
}

void GGuideLight::setRed(){
    setColor(Red);
}

void GGuideLight::setYellow(){
    setColor(Yellow);
}

void GGuideLight::setGreen(){
    setColor(Green);
}

