#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <windows.h>
#include "gplot.h"
#include "gdatabase.h"
#include "gcomponent.h"

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
    OverallPlot *SmallyOverallPlot;

};

#endif // MAINWINDOW_H
