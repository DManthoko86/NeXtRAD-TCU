#ifndef EXPERIMENTGUI_H
#define EXPERIMENTGUI_H

#include <QDialog>
#include <QTime>
#include <QDateTime>
#include "rhino.h"

namespace Ui {
class ExperimentGui;
}

class ExperimentGui : public QDialog
{
    Q_OBJECT

public:
    explicit ExperimentGui(QWidget *parent = 0);
    ~ExperimentGui();

    // Variables
    unsigned int n, m;
    QDateTime startTime, endTime;

    void setPulsePtr(prf *pulsePtr);
    void setPulsesInBackend(void);

    void setExperimentInGui(unsigned int newN, unsigned int newM, QDateTime newStartTime, QDateTime newEndTime);

private:
    Ui::ExperimentGui *ui;
    struct prf* pulses;

private slots:
    void on_pushButton_clicked();
    void setPulsesInGui(void);

    void on_pushButton_2_clicked();

signals:
    void setExperimentInBackend(void);
};

#endif // EXPERIMENTGUI_H
