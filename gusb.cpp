#include "gusb.h"

GUSB::GUSB(QObject *parent):
    QObject (parent)
{
    usb_init();
    usb_find_busses();
    usb_find_devices();

    this->getDeviceList();
}

GUSB::~GUSB(){}

void GUSB::getDeviceList()
{
    struct usb_bus *my_usbbus;
    struct usb_device *my_usbdevice;
    for(my_usbbus = usb_get_busses(); my_usbbus;
        my_usbbus = my_usbbus->next)
        for (my_usbdevice = my_usbbus->devices; my_usbdevice;
             my_usbdevice = my_usbdevice->next)
        {
            qDebug()<<"VID:VID = %x:%x\n"<<my_usbdevice->descriptor.idVendor<<
                      my_usbdevice->descriptor.idProduct;
        }
}
