#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Smally");
    setMinimumSize(800, 600);   //Set size of main widget
    flagYAxisLog = false;
    flagXAxisLog = false;

    SmallySpectral    = new Spectral(this);
    SmallyOverallPlot = new OverallPlot(this);
    SmallyMainThread  = new TimeThread(this);
    SmallyFileSys     = new FileProcessor(this, SmallySpectral);

    //Set size policy
    QSizePolicy OAPlotPolicy = SmallyOverallPlot->sizePolicy();
    QSizePolicy LeftToolPolicy = ui->LeftTool->sizePolicy();
    OAPlotPolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    LeftToolPolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    SmallyOverallPlot->setSizePolicy(OAPlotPolicy);
    ui->LeftTool->setSizePolicy(LeftToolPolicy);

    //Set splitter line style
    /*
    ui->splitter->setStyleSheet(
                "QSplitter::handle{background-color:gray}");
    ui->splitter->setHandleWidth(1);
    QSplitterHandle *handle = ui->splitter->handle(2);
    if(handle)
        handle->setFixedHeight(1);
    */

    //Add component to widget
    ui->MidLay->addWidget(SmallyOverallPlot->OASlider);
    ui->MidLay->addWidget(SmallyOverallPlot);
    ui->MidLay->addWidget(SmallyOverallPlot->AttachedPlot);

    //Set size of Left, Mid and Right layout
    ui->splitter->setStretchFactor(0,6);
    ui->splitter->setStretchFactor(1,1);
    ui->splitter_2->setStretchFactor(0,1);
    ui->splitter_2->setStretchFactor(1,3);
    qDebug()<<"Mainwindow Created";

    /***Connect Slots and Signals***/
    connect(ui->DotBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setDotDisplay);
    connect(ui->DotBox,         &QCheckBox::clicked,
            this,               &MainWindow::showSpectral);
    connect(ui->ThreadBox,      &QCheckBox::clicked,
            SmallyMainThread,   &TimeThread::setTimeThread);
    //Set action button
    connect(ui->actionStart,    &QAction::triggered,
            ui->actionPause,    &QAction::setDisabled);
    connect(ui->actionStart,    &QAction::triggered,
            ui->actionStart,    &QAction::setEnabled);
    connect(ui->actionPause,    &QAction::triggered,
            ui->actionPause,    &QAction::setEnabled);
    connect(ui->actionPause,    &QAction::triggered,
            ui->actionStart,    &QAction::setDisabled);
    connect(ui->actionClear,    &QAction::triggered,
            ui->actionPause,    &QAction::setEnabled);
    connect(ui->actionClear,    &QAction::triggered,
            ui->actionStart,    &QAction::setDisabled);

    connect(ui->actionClear,    &QAction::triggered,
            SmallySpectral,     &Spectral::Reset);
    connect(ui->actionStart,    &QAction::triggered,
            SmallyMainThread,   &TimeThread::startTimeThread);
    connect(ui->actionPause,    &QAction::triggered,
            SmallyMainThread,   &TimeThread::pauseTimeThread);

    connect(SmallySpectral,     &Spectral::Changed,
            this,               &MainWindow::showSpectral);

    connect(ui->actionNew,      &QAction::triggered,
            SmallyFileSys,      &FileProcessor::creatFile);
    connect(ui->actionSave,     &QAction::triggered,
            SmallyFileSys,      &FileProcessor::saveFile);
    connect(ui->actionSave_as,  &QAction::triggered,
            SmallyFileSys,      &FileProcessor::saveFile_as);
    connect(ui->actionOpen,     &QAction::triggered,
            SmallyFileSys,      &FileProcessor::openFile);

    //Set refresh
    connect(SmallyMainThread,   &TimeThread::Timeout100ms,
            this,               &MainWindow::showSpectral);
    connect(SmallyMainThread,   &TimeThread::Timeout50ms,
            this,               &MainWindow::setDataSeries);
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
    ui->actionPause->setDisabled(true);
    SmallyMainThread->start();
}

MainWindow::~MainWindow()
{
    delete SmallyFileSys;
    SmallyMainThread->deleteLater();
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

void MainWindow::setDataSeries()
{
    uint buff = 0;
    for(int counter = 0; counter < 1000; counter++)
    {
        buff = (rand() % (1023-0+1))+ 0;
        SmallySpectral->ReceiveCount(buff);
    }
}
