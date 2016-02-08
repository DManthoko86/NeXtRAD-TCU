#include "rhino.h"

Rhino::Rhino()
{

}


// Connection functions

// Finds out if Rhino is online. If so, the following are tested
// If online,                               returns 1
// If online and waiting for trigger pulse, returns 2
// If online and has trigger,               returns 3
// If online and the experiment is over,    rethrns 4



// Finds out if Rhino is online. If so, the following are tested
// If online,                               returns 1
// If online and the experiment is over,    returns 2
// If online and waiting for trigger pulse, returns 3
int Rhino::connect(void)
{
    char line[128];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(ip.c_str(), port);
    string message;
    int len;

    if (stream) {
        message = "Are you still alive?";
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);

        delete stream;

        // Tests that Rhino is responding
        if(line[0] == 'Y')
        {
            char status = retrieveStatus() + 1;
            return (int)status;
        }
        else                return 0;
    }

    return 0;
}

int Rhino::retrieveN(void)
{
    char line[128];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(ip.c_str(), port);
    string message;
    int len;
    int n;

    if (stream) {
        message = "N";
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);

        delete stream;

        // Tests that retrieved message is indeed N
        if(line[0] == 'N')
        {
            // Changes N to the retrieved N
            n = line[1];
        }
        return n;
    }
    else return 0;
}

void Rhino::transmitMessage(char* line)
{
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(ip.c_str(), port);

    // Transmit line
    if (stream) {
        stream->send(line, 409);
        int len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);

        delete stream;
    }
    else
    {
        printf("Connection refused.");
    }
}

char Rhino::retrieveStatus()
{
    char line[128];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(ip.c_str(), port);
    string message;
    int len;
    int status;

    if (stream) {
        message = "S";
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);

        delete stream;

        // Tests that retrieved message is indeed the status
        if(line[0] == 'S')
        {
            status = line[1];
        }
        return status;
    }
    else return 0;
}


