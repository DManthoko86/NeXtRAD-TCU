#include "rhinogui.h"
#include "ui_rhinogui.h"

rhinoGui::rhinoGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rhinoGui)
{
    ui->setupUi(this);
}

rhinoGui::~rhinoGui()
{
    delete ui;
}

void rhinoGui::setIp(string ip)
{
    QString newIp = QString::fromStdString(ip);
    ui->lineEditIp->setText(newIp);
}

void rhinoGui::setPort(int port)
{
    QString newPort = QString::number(port);
    ui->lineEditPort->setText(newPort);
}

void rhinoGui::setOnlineStatus(int status)
{
    if(status >= 1)
    {
        ui->labelRefresh->setText("Rhino is online");
        ui->pushButtonSend->setEnabled(true);

        // If online,                               status = 1
        // If online and the experiment is over,    status = 2
        // If online and waiting for trigger pulse, status = 3
        if(status == 1)
        {
            ui->labelStatus->setText("Rhino is online and not waiting for a trigger.");
        }
        else if(status == 4)
        {
            ui->labelStatus->setText("Rhino is online and the experiment is over.");
        }
        else if(status == 3)
        {
            ui->labelStatus->setText("Rhino is online and waiting for a trigger.");
        }

    }
    else
    {
        ui->labelRefresh->setText("Rhino is offline.");
        ui->pushButtonSend->setEnabled(false);
        ui->labelStatus->setText("Rhino is offline.");
    }

    ui->labelStatusNum->setText("Status number: " + QString::number(status));
}


//string getIp(void);
//int getPort(void);
void rhinoGui::on_buttonBox_accepted()
{
    emit acceptedRhinoChanges(rhinoNumber, ui->lineEditIp->text().toStdString(), ui->lineEditPort->text().toInt());
}

void rhinoGui::on_pushButtonRefresh_clicked()
{
    emit acceptedRhinoChanges(rhinoNumber, ui->lineEditIp->text().toStdString(), ui->lineEditPort->text().toInt());
}

void rhinoGui::on_pushButtonSend_clicked()
{
    emit transmitAllParameters(rhinoNumber);
}

void rhinoGui::on_pushButtonArm_clicked()
{
    emit armTCU(rhinoNumber);
    emit acceptedRhinoChanges(rhinoNumber, ui->lineEditIp->text().toStdString(), ui->lineEditPort->text().toInt());
}

void rhinoGui::on_pushButtonStop_clicked()
{
    emit stopTCU(rhinoNumber);
    emit acceptedRhinoChanges(rhinoNumber, ui->lineEditIp->text().toStdString(), ui->lineEditPort->text().toInt());
}
