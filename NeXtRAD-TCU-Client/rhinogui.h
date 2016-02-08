#ifndef RHINOGUI_H
#define RHINOGUI_H

#include <QDialog>
#include <string>


namespace Ui {
class rhinoGui;
}

using namespace std;

class rhinoGui : public QDialog
{
    Q_OBJECT

public:
    explicit rhinoGui(QWidget *parent = 0);
    ~rhinoGui();
    void setIp(string ip);
    void setPort(int port);
    void setOnlineStatus(int status);   //status is 1=connected
    int rhinoNumber = -1;

private:
    Ui::rhinoGui *ui;

signals:
    void acceptedRhinoChanges(int rhinoNumber, string newIp, int newPort);
    void transmitAllParameters(int rhinoNumber);
    void armTCU(int rhinoNumber);
    void stopTCU(int rhinoNumber);

private slots:
    void on_buttonBox_accepted();
    void on_pushButtonRefresh_clicked();
    void on_pushButtonSend_clicked();
    void on_pushButtonArm_clicked();
    void on_pushButtonStop_clicked();
};

#endif // RHINOGUI_H
