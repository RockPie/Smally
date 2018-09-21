#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Smally");
    resize(600,450);
    SmallyOverallPlot = new OverallPlot(this);
    ui->MainLay->addWidget(SmallyOverallPlot);

    Sleep(1000);                                    //To show splash screen
    Spectral F;
    F.ReceiveCount(2055);
}

MainWindow::~MainWindow()
{
    delete ui;
}
