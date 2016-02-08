#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "ui_mainwindow.h"
//GUIs
#include "rhinogui.h"
#include "experimentgui.h"

#include "backend.h"
#include <string>
#include <iostream>


using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    backend Backend;
    void openExperimentSettings();
    void openRhinoSettings(int rhinoNumber);
    rhinoGui *r;     // Rhino settings window
    ExperimentGui *experiment;
    ~MainWindow();

private slots:
    void rhinoChangeSettings(int rhinoNumber, string newIp, int newPort);
    void rhinoTransmitAllParameters(int rhinoNumber);
    void setExperimentInBackend(void);
    void armTCU(int rhinoNumber);
    void stopTCU(int rhinoNumber);

    void on_actionImport_Header_File_triggered();
    void on_rhino0_clicked();
    void on_rhino1_clicked();
    void on_rhino2_clicked();
    void on_actionImport_Settings_triggered();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void changeRhinoIp(int n);
    //rhinoGui *r;
};


#endif // MAINWINDOW_H
