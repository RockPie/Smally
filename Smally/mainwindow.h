#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <windows.h>
#include "gplot.h"
#include "gdatabase.h"
#include "gcomponent.h"
#include "gthread.h"

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

    bool flagYAxisLog;
    bool flagXAxisLog;

private slots:
    void showSpectral();
    void setYLogAxis(bool isLog);
    void setXLogAxis(bool isLog);


};

#endif // MAINWINDOW_H
