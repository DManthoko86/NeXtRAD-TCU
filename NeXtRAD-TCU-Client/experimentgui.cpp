#include "experimentgui.h"
#include "ui_experimentgui.h"

ExperimentGui::ExperimentGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentGui)
{
    ui->setupUi(this);
}

ExperimentGui::~ExperimentGui()
{
    delete ui;
}

// Points pulses here to the pulses in mainwindow
void ExperimentGui::setPulsePtr(prf * pulsePtr)
{
    pulses = pulsePtr;
    setPulsesInGui();
}

void ExperimentGui::setPulsesInBackend()
{
    // pulseNo is the pulse number currently selected
    int pulseNo = ui->spinBoxPulseNo->value();

    pulses[pulseNo].mb        = ui->spinBoxMB->value()/10;
    pulses[pulseNo].dig       = ui->spinBoxDIG->value()/10;
    pulses[pulseNo].pri       = ui->spinBoxPRI->value()/10;
    pulses[pulseNo].frequency = ui->spinBoxFrequency->value();
    pulses[pulseNo].mode      = ui->comboBoxMode->currentIndex();
    pulses[pulseNo].notused   = ui->spinBoxMB->value();
}

void ExperimentGui::setPulsesInGui()
{
    // pulseNo is the pulse number currently selected
    int pulseNo = ui->spinBoxPulseNo->value();

    ui->spinBoxMB->setValue(pulses[pulseNo].mb*10);
    ui->spinBoxDIG->setValue(pulses[pulseNo].dig*10);
    ui->spinBoxPRI->setValue(pulses[pulseNo].pri*10);
    ui->spinBoxFrequency->setValue(pulses[pulseNo].frequency);
    ui->spinBoxNotused->setValue(pulses[pulseNo].notused);
    ui->comboBoxMode->setCurrentIndex(pulses[pulseNo].mode);
}

void ExperimentGui::setExperimentInGui(unsigned int newN, unsigned int newM, QDateTime newStartTime, QDateTime newEndTime)
{
    ui->spinBoxPulses->setValue(newN);
    ui->spinBoxRepeats->setValue(newM);
    ui->dateTimeEditStartTime->setDateTime(newStartTime);
    ui->dateTimeEditEndTime->setDateTime(newEndTime);
}

void ExperimentGui::on_pushButton_clicked()
{
    setPulsesInBackend();
}

void ExperimentGui::on_pushButton_2_clicked()
{
    n = ui->spinBoxPulses->value();
    m = ui->spinBoxRepeats->value();
    startTime = ui->dateTimeEditStartTime->dateTime();
    endTime = ui->dateTimeEditEndTime->dateTime();
    emit setExperimentInBackend();
}
