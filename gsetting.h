#ifndef GSETTING_H
#define GSETTING_H

//#define uint unsigned int

#define ChannelNum 1024
#define OverallYIniMax 1000

#define OAMinLinePos 0
#define OAMaxLinePos ChannelNum - 1
#define OAInitialVal 0

#define ScaleThresholdH 0.9
#define ScaleThresholdL 0.3
#define GridNum 10

#define RecWidth 45
#define RecLength 140
#define HalfWidth 20

#define USB_clear 0xFFCC
#define USB_pause 0xFFDD
#define USB_start 0xFFEE
#define USB_fetch 0xFFFF

#define USB_title 0xFFFF
#define USB_VID   0x0471
#define USB_PID   0x0666

#define EP_IN     0x82
#define EP_OUT    0x01

#define FileInfo "Smally Spectral version 0.01"



#endif // GSETTING_H
