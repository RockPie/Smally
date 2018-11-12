#ifndef GUSB_H
#define GUSB_H
#include <QObject>
#include <QDebug>
#include "lusb0_usb.h"
#include "gsetting.h"

class GUSB: public QObject
{
    Q_OBJECT
public:
    GUSB(QObject *parent = nullptr);
    ~GUSB();
    void InitUSB();

signals:
    void DevFound();

public slots:
    void clearUSB();
    void startUSB();
    void fetchUSB();
    void pauseUSB();

private:
    usb_dev_handle *open_dev(void);

private:
    struct usb_bus *my_usbbus;
    struct usb_device *my_usbdevice;
    struct usb_dev_handle *my_udev;

    quint64 USBcounter[ChannelNum];

signals:
    void submitRes(quint64* val);
};

#endif // GUSB_H
