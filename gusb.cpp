#include "gusb.h"

GUSB::GUSB(QObject *parent):
    QObject (parent){}

GUSB::~GUSB(){
    usb_close(my_udev);
}

void GUSB::InitUSB()
{
    usb_init();
    usb_find_busses();
    usb_find_devices();

    if (!(my_udev = open_dev())) {
        qDebug()<<"error opening device";
    }
    else{
        qDebug()<<"USB open success";
        emit DevFound();
    }
}


usb_dev_handle *GUSB::open_dev(void)
{
    for(my_usbbus = usb_get_busses(); my_usbbus;
        my_usbbus = my_usbbus->next)
    {
        for (my_usbdevice = my_usbbus->devices; my_usbdevice;
             my_usbdevice = my_usbdevice->next)
        {
            qDebug()<<"VID:VID = "<<my_usbdevice->descriptor.idVendor<<
                      my_usbdevice->descriptor.idProduct;
            return usb_open(my_usbdevice);
        }
    }
    return nullptr;
}

void GUSB::clearUSB(){
    usb_claim_interface(my_udev, 0);
    int order = USB_clear;
    usb_bulk_write(my_udev, EP_OUT, (char *)&order, 2, 5000);
}

void GUSB::startUSB(){
    usb_claim_interface(my_udev, 0);
    int order = USB_start;
    usb_bulk_write(my_udev, EP_OUT, (char*)&order, 2, 5000);
}

void GUSB::fetchUSB(){
    int EffectiveLoc = 0;
    usb_claim_interface(my_udev, 0);
    int order = USB_fetch;
    usb_bulk_write(my_udev, EP_OUT, (char*)&order, 2, 5000);
    char rev[8*ChannelNum] = {0};
    usb_bulk_read(my_udev, EP_IN, rev, 8*ChannelNum, 5000);
    for(int counter = 0; counter < 8*ChannelNum; counter++)
        if (rev[counter] == -1)
        {
            EffectiveLoc = counter;
            break;
        }
    for(int counter = EffectiveLoc; counter < ChannelNum; counter++)
    {
//        QString tmp[4] = {"0"};
//        tmp[0] = QString::number(int(rev[counter*8 + 4]));
//        tmp[1] = QString::number(int(rev[counter*8 + 5]));
//        tmp[2] = QString::number(int(rev[counter*8 + 6]));
//        tmp[3] = QString::number(int(rev[counter*8 + 7]));
//        qDebug()<<tmp[0]<<tmp[1]<<tmp[2]<<tmp[3];
//        qDebug()<<tmp[2] + tmp[3] + tmp[0] + tmp[1];
        //USBcounter[counter] = (tmp[2] + tmp[3] + tmp[0] + tmp[1]).toLongLong();
        quint64 tmp = quint64(double(rev[counter*8 + 6]) * 256 * 256 * 256 + double(rev[counter*8 + 7]) * 256 * 256
                      + double(rev[counter*8 + 4]) *256 + double(rev[counter*8 + 5]));
        //qDebug()<<tmp;
        USBcounter[counter] = tmp;
        emit submitRes(USBcounter);

        //qDebug()<<quint64(rev[counter*8 + 6])<<quint64(rev[counter*8 + 7])<<quint64(rev[counter*8 + 4])<<quint64(rev[counter*8 + 5]);
    }
}

void GUSB::pauseUSB(){
    usb_claim_interface(my_udev, 0);
    int order = USB_pause;
    usb_bulk_write(my_udev, EP_OUT, (char*)&order, 2, 5000);
}
