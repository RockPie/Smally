#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Smally");
    resize(600,450);
    SmallySpectral    = new Spectral(this);
    SmallyOverallPlot = new OverallPlot(this);
    SmallyMainThread  = new TimeThread(this);

    //Set size policy
    QSizePolicy OAPlotPolicy = SmallyOverallPlot->sizePolicy();
    QSizePolicy LeftToolPolicy = ui->LeftTool->sizePolicy();
    OAPlotPolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    LeftToolPolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    SmallyOverallPlot->setSizePolicy(OAPlotPolicy);
    ui->LeftTool->setSizePolicy(LeftToolPolicy);

    //Set splitter line style
    ui->splitter->setStyleSheet(
                "QSplitter::handle{background-color:gray}");
    ui->splitter->setHandleWidth(1);
    QSplitterHandle *handle = ui->splitter->handle(2);
    if(handle)
        handle->setFixedHeight(1);

    //Set size of Left, Mid and Right layout
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,4);
    ui->splitter->setStretchFactor(2,1);

    //Add component to widget
    ui->MidLay->addWidget(SmallyOverallPlot->OASlider);
    ui->MidLay->addWidget(SmallyOverallPlot);
    qDebug()<<"Mainwindow Created";

    /***Connect Slots and Signals***/
    connect(ui->DotBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setDotDisplay);
    connect(ui->ThreadBox,      &QCheckBox::clicked,
            SmallyMainThread,   &TimeThread::setTimeThread);

    //Sleep(500);   //To show splash screen
}

MainWindow::~MainWindow()
{
    delete SmallyMainThread;
    delete SmallyOverallPlot;
    delete SmallySpectral;
    delete ui;
}
