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
    isLinked = false;
    isSimSource = true;

    SmallySpectral    = new Spectral(this);
    SmallyOverallPlot = new OverallPlot(this);
    SmallyMainThread  = new TimeThread(this);
    SmallyFileSys     = new FileProcessor(this, SmallySpectral);
    USBLight          = new GGuideLight(this);
    StatusLight       = new GGuideLight(this);
    TCPInfo           = new QLabel(this);
    StatusInfo        = new QLabel(this);
    DataSource        = new DataEngine(this);
    SmallyClient      = new GNetwork(this);
    qDebug()<<"Component Created";

    //Set size policy
    QSizePolicy OAPlotPolicy = SmallyOverallPlot->sizePolicy();
    QSizePolicy LeftToolPolicy = ui->LeftTool->sizePolicy();
    OAPlotPolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    LeftToolPolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    SmallyOverallPlot->setSizePolicy(OAPlotPolicy);
    ui->LeftTool->setSizePolicy(LeftToolPolicy);

    //Add component to widget
    ui->MidLay->addWidget(SmallyOverallPlot->OASlider);
    ui->MidLay->addWidget(SmallyOverallPlot);
    ui->MidLay->addWidget(SmallyOverallPlot->AttachedPlot);
    ui->statusBar->addWidget(USBLight);
    ui->statusBar->addWidget(TCPInfo);
    ui->statusBar->addWidget(StatusLight);
    ui->statusBar->addWidget(StatusInfo);

    CurveTitle = SmallySpectral->getInfo();
    SmallyOverallPlot->setTitle(CurveTitle);

    //Set size of Left, Mid and Right layout
    ui->splitter->setStretchFactor(0,8);
    ui->splitter->setStretchFactor(1,1);
    ui->splitter_2->setStretchFactor(0,1);
    ui->splitter_2->setStretchFactor(1,3);
    qDebug()<<"Mainwindow Created";

    /***Connect Slots and Signals***/
    connect(ui->AutoBox,        &QCheckBox::clicked,
            SmallyOverallPlot,  &OverallPlot::setAutoScale);

    //Set action button
    connect(ui->actionClear,    &QAction::triggered,
            SmallySpectral,     &Spectral::Reset);
    connect(ui->actionStart,    &QAction::triggered,
            SmallyMainThread,   &TimeThread::startTimeThread , Qt::QueuedConnection);
    connect(ui->actionPause,    &QAction::triggered,
            SmallyMainThread,   &TimeThread::pauseTimeThread , Qt::QueuedConnection);

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
    connect(SmallySpectral,     &Spectral::SpecChanged,
            this,               &MainWindow::showSpectral);
    connect(SmallySpectral,     &Spectral::NucChanged,
            this,               &MainWindow::updatePlotTitle);

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
            this,               &MainWindow::showSpectral , Qt::QueuedConnection);

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

    //Link Host Info input
    connect(ui->hostlineEdit,   &QLineEdit::textChanged,
            SmallyClient,       &GNetwork::setHostName, Qt::QueuedConnection);
    connect(ui->portlineEdit,   &QLineEdit::textChanged,
            SmallyClient,       &GNetwork::setPortNum, Qt::QueuedConnection);

    connect(SmallyClient,       &GNetwork::newConnectionSet,
            USBLight,           &GGuideLight::setGreen , Qt::QueuedConnection);
    connect(SmallyClient,       &GNetwork::newConnectionSet,
            this,               &MainWindow::showHostLinked , Qt::QueuedConnection);
    connect(SmallyClient,       &GNetwork::errorMet,
            this,               &MainWindow::showLinkError , Qt::QueuedConnection);
    connect(SmallyClient,       &GNetwork::connectionDisabled,
            this,               &MainWindow::showHostDisconnected , Qt::QueuedConnection);

    connect(ui->linkButton,     &QPushButton::clicked,
            this,               &MainWindow::changeButton);
    connect(this,               &MainWindow::startLink,
            SmallyClient,       &GNetwork::newConnect , Qt::QueuedConnection);
    connect(this,               &MainWindow::stopLink,
            SmallyClient,       &GNetwork::disconnect , Qt::QueuedConnection);
    connect(ui->SourcecomboBox, &QComboBox::currentTextChanged,
            this,               &MainWindow::setSourceType);
    connect(ui->SimcomboBox,    SIGNAL(currentIndexChanged(int)),
            DataSource,         SLOT(setDataNum(int)));

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
    USBLight->update();
    StatusLight->update();
    this->showSYScleared();
    this->showHostDisconnected();
    ui->hostlineEdit->setText("localhost");
    ui->portlineEdit->setText("6666");
    setSourceType("模拟信号源");
}

MainWindow::~MainWindow()
{
    SmallyClient->deleteLater();
    DataSource->deleteLater();
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
}

void MainWindow::setYLogAxis(bool isLog){
    flagYAxisLog = isLog;
}

void MainWindow::setXLogAxis(bool isLog){
    flagXAxisLog = isLog;
}

void MainWindow::setDataSeries(int val){
    SmallySpectral->ReceiveCount(uint(val));
}

void MainWindow::showSYSstarted(){
    DataSource->pause.unlock();
    StatusInfo->setText("Collecting Started");
    StatusInfo->update();
}

void MainWindow::showSYSpaused(){
    DataSource->pause.lock();
    StatusInfo->setText("Collecting Paused");
    StatusInfo->update();
}

void MainWindow::showSYScleared(){
    StatusInfo->setText("Data Cleaned");
    StatusInfo->update();
}

void MainWindow::showHostLinked(){
    TCPInfo->setText("TCP Connected");
    TCPInfo->update();
}

void MainWindow::showLinkError(QString info){
    USBLight->setYellow();
    TCPInfo->setText(info);
    TCPInfo->update();
}

void MainWindow::showHostDisconnected(){
    USBLight->setRed();
    TCPInfo->setText("TCP Disconnected");
    TCPInfo->update();
}

void MainWindow::changeButton()
{
    if(isLinked)
    {
        ui->linkButton->setText("连接");
        isLinked = false;
        emit stopLink();
    }
    else
    {
        ui->linkButton->setText("断开连接");
        isLinked = true;
        emit startLink();
    }
}

void MainWindow::setSourceType(QString type)
{
    if(type == "模拟信号源")
    {
        connect(DataSource,         &DataEngine::SimulateEvent,
                this,               &MainWindow::setDataSeries , Qt::QueuedConnection);
        ui->linkButton->setEnabled(false);
        SmallySpectral->setElement("Example");
        SmallySpectral->setNucNum(0);
        ui->SimcomboBox->setEnabled(true);
    }
    else if(type == "外部服务器")
    {
        disconnect(DataSource,         &DataEngine::SimulateEvent,
                   this,               &MainWindow::setDataSeries);
        ui->linkButton->setEnabled(true);
        SmallySpectral->setElement("Unknown");
        SmallySpectral->setNucNum(0);
        ui->SimcomboBox->setEnabled(false);
    }
}

void MainWindow::updatePlotTitle(){
    SmallyOverallPlot->setTitle(SmallySpectral->Element + "-"
                                + QString::number(SmallySpectral->NucleonNum));
}


