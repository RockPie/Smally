#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <windows.h>
#include <QTime>
#include "gplot.h"
#include "gdatabase.h"
#include "gfile.h"
#include "gcomponent.h"
#include "gthread.h"
#include "gclient.h"
#include "gusb.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Spectral *SmallySpectral;
    OverallPlot *SmallyOverallPlot;
    TimeThread *SmallyMainThread;
    FileProcessor *SmallyFileSys;

    QString CurveTitle;
    GGuideLight *USBLight;
    GGuideLight *StatusLight;
    QLabel *TCPInfo;
    QLabel *StatusInfo;
    DataEngine *DataSource;
    GNetwork *SmallyClient;
    QTime TimeBuffer;
    GUSB *SmallyUSB;
    bool flagYAxisLog;
    bool flagXAxisLog;
    bool isLinked;
    bool isSimSource;
    bool isCountingDown;

signals:
    void startLink();
    void stopLink();

private slots:
    void showSpectral();
//    void updatePlotTitle();
    void setYLogAxis(bool isLog);
    void setXLogAxis(bool isLog);
    void setDataSeries(int val);

    void stopWatch();

    void showSYSstarted();
    void showSYSpaused();
    void showSYScleared();

    void showHostLinked();
    void showHostDisconnected();
    void showLinkError(QString info);

    void changeButton();
    void onUSBAutoClicked(bool isAuto);

    void setSourceType(QString type);
    void USBDeviceFound();
};

#endif // MAINWINDOW_H
