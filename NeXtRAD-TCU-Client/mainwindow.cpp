#include "mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/////////////////
// CUSTOM FUNCTIONS
/////////////////

void MainWindow::openExperimentSettings()
{
    experiment = new ExperimentGui();
    experiment->setModal(true);
    experiment->setWindowTitle("Experiment Header settings");
    experiment->setPulsePtr(Backend.pulses);
    experiment->setExperimentInGui(Backend.n, Backend.m, Backend.startTime, Backend.endTime);
    //experiment->setExperimentInGui(3, 3, Backend.startTime, Backend.endTime);

    connect(experiment, SIGNAL(setExperimentInBackend()), this, SLOT(setExperimentInBackend()));

    experiment->exec();
}

void MainWindow::openRhinoSettings(int rhinoNumber)
{
    r = new rhinoGui;
    r->setModal(true);
    r->setWindowTitle(QString("Rhino %1").arg(rhinoNumber)); // Title of window
    r->rhinoNumber = rhinoNumber;                            // Either 0, 1 or 2 (node)
    r->setIp(Backend.rhino[rhinoNumber].ip);                 // Changes IP in text field to be known IP
    r->setPort(Backend.rhino[rhinoNumber].port);             // changes Port in text feild to be what is known

    r->setOnlineStatus(Backend.rhino[rhinoNumber].connect());

    // Signals - Slots
    connect(r, SIGNAL(acceptedRhinoChanges(int,string,int)), this, SLOT(rhinoChangeSettings(int,string,int)));
    connect(r, SIGNAL(transmitAllParameters(int)), this, SLOT(rhinoTransmitAllParameters(int)));

    r->exec();
}

// SLOTS


/////////////////
// Menu Items
/////////////////unsigned
void MainWindow::on_actionImport_Header_File_triggered()
{
    // Open file dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Header File"), ".", tr("Header Files (*.ini *.txt)"));
    Backend.importExperiment(fileName.toStdString());
}

void MainWindow::on_actionImport_Settings_triggered()
{
    // Open file dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Settings file"), ".", tr("Header Files (*.ini *.txt)"));
    Backend.importNetworkSettings(fileName.toStdString());
}

/////////////////
// End Menu Items
/////////////////

void MainWindow::rhinoChangeSettings(int rhinoNumber, string newIp, int newPort)
{
    Backend.rhino[rhinoNumber].ip = newIp;
    Backend.rhino[rhinoNumber].port = newPort;

    r->setOnlineStatus(Backend.rhino[rhinoNumber].connect());
}

void MainWindow::rhinoTransmitAllParameters(int rhinoNumber)
{
    Backend.rhinoTransmitParameters(rhinoNumber);
}

void MainWindow::setExperimentInBackend()
{
    Backend.n = experiment->n;
    Backend.m = experiment->m;
    Backend.startTime = experiment->startTime;
    Backend.endTime = experiment->endTime;
}


// Opens up settings windows
void MainWindow::on_rhino0_clicked()        {openRhinoSettings(0);}
void MainWindow::on_rhino1_clicked()        {openRhinoSettings(1);}
void MainWindow::on_rhino2_clicked()        {openRhinoSettings(2);}



void MainWindow::on_pushButton_clicked()    {openExperimentSettings();}
