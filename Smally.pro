#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T23:07:24
#
#-------------------------------------------------

QT          += core gui widgets network

#CONFIG      += qwt
DEFINES     += QT_DLL QWT_DLL

# When releasing, use -lqwt.
#LIBS        += -L"C:\Qt\Qwt\build-qwt-Desktop_Qt_5_11_1_MinGW_32bit-Debug\lib" -lqwt
# When debugging, use -lqwtd.
#LIBS        += -L"C:\Qt\Qwt\build-qwt-Desktop_Qt_5_11_1_MinGW_32bit-Debug\lib" -lqwtd

INCLUDEPATH += C:\Qt\5.11.1\mingw53_32\include\QWT

TARGET = Smally
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += console

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gplot.cpp \
    gdatabase.cpp \
    gcomponent.cpp \
    gthread.cpp \
    gfile.cpp \
    gpaintingsystem.cpp \
    gclient.cpp \
    newfile_dialog.cpp

HEADERS += \
        mainwindow.h \
    gplot.h \
    gsetting.h \
    gdatabase.h \
    gcomponent.h \
    gthread.h \
    gfile.h \
    gpaintingsystem.h \
    gclient.h \
    newfile_dialog.h

FORMS += \
        mainwindow.ui \
    newfile_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    icon.qrc \
    bispec.qrc
