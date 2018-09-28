#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Smally");
    resize(800, 600);   //Set size of main widget
    flagYAxisLog = false;
    flagXAxisLog = false;

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
    ui->MidLay->addWidget(SmallyOverallPlot->AttachedPlot);
    qDebug()<<"Mainwindow Created";

    /***Connect Slots and Signals***/
    connect(ui->DotBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setDotDisplay);
    connect(ui->DotBox,         &QCheckBox::clicked,
            this,               &MainWindow::showSpectral);
    connect(ui->ThreadBox,      &QCheckBox::clicked,
            SmallyMainThread,   &TimeThread::setTimeThread);
    connect(SmallyMainThread,   &TimeThread::Timeout100ms,
            this,               &MainWindow::showSpectral);
    //Link logbox
    connect(ui->LogXBox,        &QCheckBox::clicked,
            this,               &MainWindow::setXLogAxis);
    connect(ui->LogXBox,        &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setXLogMode);
    connect(ui->LogXBox,        &QCheckBox::clicked,
            this,               &MainWindow::showSpectral);
    connect(ui->LogYBox,        &QCheckBox::clicked,
            this,               &MainWindow::setYLogAxis);
    connect(ui->LogYBox,        &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setYLogMode);
    connect(ui->LogYBox,        &QCheckBox::clicked,
            this,               &MainWindow::showSpectral);
    //Link double slider
    connect(ui->XminSpinBox,    SIGNAL(valueChanged(double)),
            SmallyOverallPlot->OASlider,
                                SLOT(setMinValue(double)));
    connect(ui->XmaxSpinBox,    SIGNAL(valueChanged(double)),
            SmallyOverallPlot->OASlider,
                                SLOT(setMaxValue(double)));
    connect(SmallyOverallPlot->OASlider,
                                &DoubleSlider::maxValueChanged,
            ui->XmaxSpinBox,    &QDoubleSpinBox::setValue);
    connect(SmallyOverallPlot->OASlider,
                                &DoubleSlider::minValueChanged,
            ui->XminSpinBox,    &QDoubleSpinBox::setValue);
    qDebug()<<"Signals and slots connected";

    ui->XmaxSpinBox->setValue(ChannelNum - 1);
    ui->XminSpinBox->setValue(0);
    ui->LogXBox->setCheckState(Qt::Unchecked);
    ui->LogYBox->setCheckState(Qt::Unchecked);
    ui->DotBox ->setCheckState(Qt::Unchecked);

    //Sleep(500);   //To show splash screen
}

MainWindow::~MainWindow()
{
    delete SmallyMainThread;
    delete SmallyOverallPlot;
    delete SmallySpectral;
    delete ui;
}

void MainWindow::showSpectral(){
    SmallyOverallPlot->OADataReceive(
                SmallySpectral->PointOAOutput(flagXAxisLog,
                                              flagYAxisLog));
    //qDebug()<<"Updated";
}

void MainWindow::setYLogAxis(bool isLog){
    flagYAxisLog = isLog;
}

void MainWindow::setXLogAxis(bool isLog){
    flagXAxisLog = isLog;
}
