#ifndef GCOMPONENT_H
#define GCOMPONENT_H


#include <QSlider>
#include <QPainter>
#include <QMouseEvent>
#include <QtGui>
#include "gsetting.h"

class DoubleSlider : public QWidget
{
    Q_OBJECT
public:
    explicit DoubleSlider(QWidget* parent);
    void setRange(double min, double max);
    void setSingleStep(double step);

    inline double minValue() const;
    inline double maxValue() const;
    inline double minRange() const;
    inline double maxRange() const;

    enum State{MinHandle, MaxHandle, None};

public slots:
    void setLabel(const QString& label);
    void setMaxValue(double val);
    void setMinValue(double val);

signals:
    void minValueChanged(double);
    void maxValueChanged(double);

private:
    double m_min;
    double m_max;
    double m_singleStep;

    double m_minValue;
    double m_maxValue;

    QRect minHandleRegion;
    QRect maxHandleRegion;

    State m_state;

    QString m_label;

protected:
    void paintEvent(QPaintEvent* event);
    inline void paintColoredRect(QRect rect, QColor color, QPainter* painter);
    void paintValueLabel(QPainter* painter);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent *event);
    void leaveEvent(QEvent* event);
};

inline double getValidValue(double val, double min, double max)
{
    double tmp = std::max(val, min);
    return std::min(tmp, max);
}

inline void DoubleSlider::paintColoredRect(
        QRect rect, QColor color ,QPainter* painter){
    painter->fillRect(rect,QBrush(color));
}

inline double DoubleSlider::maxRange() const{
    return m_max;
}

inline double DoubleSlider::minRange() const{
    return m_min;
}

inline double DoubleSlider::maxValue() const{
    return m_maxValue;
}

inline double DoubleSlider::minValue() const{
    return m_minValue;
}


#endif // GPCOMPONENT_H
