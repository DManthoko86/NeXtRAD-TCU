#ifndef RHINO_H
#define RHINO_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <string>

#include "tcpsocket/tcpconnector.h"

struct prf{
    uint16_t mb;
    uint16_t dig;
    uint16_t pri;
    uint16_t frequency;
    uint16_t mode;
    uint16_t notused;
};

class Rhino
{
// Private variables
private:

// Public variables
public:
    Rhino();
    // Connection variables
    string ip;
    int port;

    // Remote connection functions
    // Note that 'retrieve' refers to getting the parameters from the Rhino
    int connect(void);
    int retrieveN(void);
    int retrieveM(void);
    void retrieveAllParameters(void);

    // TCP functions
    void transmitMessage(char* line);
    void transmitN(void);
    void transmitM(void);

    char retrieveStatus(void);
};



#endif // RHINO_H
