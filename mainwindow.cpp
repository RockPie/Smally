#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Smally");
    setMinimumSize(800, 600);   //Set size of main widget
    qDebug()<<"Minimum Size Set";

    flagYAxisLog = false;
    flagXAxisLog = false;

    SmallySpectral    = new Spectral(this);
    SmallyOverallPlot = new OverallPlot(this);
    SmallyMainThread  = new TimeThread(this);
    SmallyFileSys     = new FileProcessor(this, SmallySpectral);
    USBLight          = new GGuideLight(this);
    StatusLight       = new GGuideLight(this);
    USBInfo           = new QLabel(this);
    StatusInfo        = new QLabel(this);
    DataSource        = new DataEngine(this);
    qDebug()<<"Component Created";

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
    ui->statusBar->addWidget(USBLight);
    ui->statusBar->addWidget(USBInfo);
    ui->statusBar->addWidget(StatusLight);
    ui->statusBar->addWidget(StatusInfo);

    CurveTitle = SmallySpectral->getInfo();
    SmallyOverallPlot->setTitle(CurveTitle);

    //Set size of Left, Mid and Right layout
    ui->splitter->setStretchFactor(0,6);
    ui->splitter->setStretchFactor(1,1);
    ui->splitter_2->setStretchFactor(0,1);
    ui->splitter_2->setStretchFactor(1,3);
    qDebug()<<"Mainwindow Created";

    /***Connect Slots and Signals***/
    connect(ui->AutoBox,        &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setAutoScale);
    connect(ui->ThreadBox,      &QCheckBox::clicked,
            SmallyMainThread,   &TimeThread::setTimeThread);

    //Set action button
    connect(ui->actionStart,    &QAction::triggered,
            ui->actionPause,    &QAction::setDisabled);
    connect(ui->actionStart,    &QAction::triggered,
            StatusLight,        &GGuideLight::setGreen);
    connect(ui->actionStart,    &QAction::triggered,
            ui->actionStart,    &QAction::setEnabled);
    connect(ui->actionStart,    &QAction::triggered,
            this,               &MainWindow::showSYSstarted);

    connect(ui->actionPause,    &QAction::triggered,
            ui->actionPause,    &QAction::setEnabled);
    connect(ui->actionPause,    &QAction::triggered,
            ui->actionStart,    &QAction::setDisabled);
    connect(ui->actionPause,    &QAction::triggered,
            StatusLight,        &GGuideLight::setYellow);
    connect(ui->actionPause,    &QAction::triggered,
            this,               &MainWindow::showSYSpaused);

    connect(ui->actionClear,    &QAction::triggered,
            ui->actionPause,    &QAction::setEnabled);
    connect(ui->actionClear,    &QAction::triggered,
            ui->actionStart,    &QAction::setDisabled);
    connect(ui->actionClear,    &QAction::triggered,
            StatusLight,        &GGuideLight::setRed);
    connect(ui->actionClear,    &QAction::triggered,
            this,               &MainWindow::showSYScleared);

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
    connect(SmallyFileSys,      &FileProcessor::NewDataLoaded,
            this,               &MainWindow::showSpectral);

    //Set refresh
    connect(SmallyMainThread,   &TimeThread::Timeout50ms,
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

    //Set 3-Points, 5-Points and DotsDisplay
    connect(ui->DotBox,         &QCheckBox::clicked,
            ui->TPFBox,         &QCheckBox::setDisabled);
    connect(ui->DotBox,         &QCheckBox::clicked,
            ui->FPFBox,         &QCheckBox::setDisabled);
    connect(ui->DotBox,         &QCheckBox::clicked,
            ui->HAFBox,         &QCheckBox::setDisabled);

    connect(ui->TPFBox,         &QCheckBox::clicked,
            ui->FPFBox,         &QCheckBox::setDisabled);
    connect(ui->TPFBox,         &QCheckBox::clicked,
            ui->HAFBox,         &QCheckBox::setDisabled);
    connect(ui->TPFBox,         &QCheckBox::clicked,
            ui->DotBox,         &QCheckBox::setDisabled);

    connect(ui->FPFBox,         &QCheckBox::clicked,
            ui->TPFBox,         &QCheckBox::setDisabled);
    connect(ui->FPFBox,         &QCheckBox::clicked,
            ui->DotBox,         &QCheckBox::setDisabled);
    connect(ui->FPFBox,         &QCheckBox::clicked,
            ui->HAFBox,         &QCheckBox::setDisabled);

    connect(ui->HAFBox,         &QCheckBox::clicked,
            ui->TPFBox,         &QCheckBox::setDisabled);
    connect(ui->HAFBox,         &QCheckBox::clicked,
            ui->FPFBox,         &QCheckBox::setDisabled);
    connect(ui->HAFBox,         &QCheckBox::clicked,
            ui->DotBox,         &QCheckBox::setDisabled);

    connect(ui->DotBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setDotDisplay);
    connect(ui->DotBox,         &QCheckBox::clicked,
            this,               &MainWindow::showSpectral);
    connect(ui->TPFBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setThreePoints);
    connect(ui->FPFBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setFivePoints);
    connect(ui->HAFBox,         &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setHighAccuracy);

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
    //Link Plot Info
    connect(SmallyOverallPlot,  &GPlot::MaxCountChanged,
            ui->MaxCount,       &QLabel::setText);
    connect(SmallyOverallPlot,  &GPlot::MinCountChanged,
            ui->MinCount,       &QLabel::setText);
    qDebug()<<"Signals and slots connected";

    ui->XmaxSpinBox->setValue(ChannelNum - 1);
    ui->XminSpinBox->setValue(0);
    ui->LogXBox->setCheckState(Qt::Unchecked);
    ui->LogYBox->setCheckState(Qt::Unchecked);
    ui->DotBox ->setCheckState(Qt::Unchecked);
    ui->AutoBox->setCheckState(Qt::Checked);
    ui->FPFBox->setCheckState(Qt::Unchecked);
    ui->TPFBox->setCheckState(Qt::Unchecked);
    ui->HAFBox->setCheckState(Qt::Unchecked);
    ui->actionPause->setDisabled(true);
    SmallyMainThread->start();
    USBLight->update();
    StatusLight->update();
    this->showSYScleared();
    this->showUSBdiaconnected();
}

MainWindow::~MainWindow()
{
    delete DataSource;
    delete StatusLight;
    delete USBLight;
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

void MainWindow::showUSBdiaconnected(){
    USBInfo->setText("USB Disconnected");
    USBInfo->update();
}

void MainWindow::showUSBconnected(){
    USBInfo->setText("USB Connected");
    USBInfo->update();
}

void MainWindow::showUSBerror(){
    USBInfo->setText("USB ERROR");
    USBInfo->update();
}

void MainWindow::showSYSstarted(){
    StatusInfo->setText("Collecting Started");
    StatusInfo->update();
}

void MainWindow::showSYSpaused(){
    StatusInfo->setText("Collecting Paused");
    StatusInfo->update();
}

void MainWindow::showSYScleared(){
    StatusInfo->setText("Data Cleaned");
    StatusInfo->update();
}
