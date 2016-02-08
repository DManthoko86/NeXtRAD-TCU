#ifndef BACKEND_H
#define BACKEND_H

#include <QString>
#include <QFileDialog>
#include <QTime>
#include <QDateTime>

#include <iostream>
#include <string>

#include "rhino.h"
#define INI_INLINE_COMMENT_PREFIXES "%"
#include "inih/cpp/INIReader.h"
#include "inih/ini.h"
#include "tcpsocket/tcpconnector.h"



class backend
{
public:
    backend();
    Rhino rhino[3];

    // Experiment data for the Rhino
    unsigned int m;
    unsigned int n;
    QDateTime startTime;
    QDateTime endTime;
    struct prf pulses[32];
    void setPulse(int n, prf newPulse);
//    void setStartTime(int newStartTime);
//    void setEndTime(int newEndTime);

    prf getPulse(int pulseNumber);

    // TCP functions
    int importNetworkSettings(string fileName);
    int importExperiment(string fileName);
    void rhinoTransmitParameters(int rhinoNumber);
    void rhinoArmTCU(int rhinoNumber);
    void rhinoStopTCU(int rhinoNumber);

private:

};

#endif // BACKEND_H
