#ifndef GUSB_H
#define GUSB_H
#include <QObject>
#include <QDebug>
#include "lusb0_usb.h"

class GUSB: public QObject
{
    Q_OBJECT
public:
    GUSB(QObject *parent = nullptr);
    ~GUSB();

    void getDeviceList();
};

#endif // GUSB_H
