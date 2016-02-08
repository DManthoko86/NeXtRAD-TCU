#include "backend.h"

backend::backend()
{
    for(int i=0; i<3; i++)
    {
        rhino[i].ip = "localhost";
        rhino[i].port = 9999;
    }
}

void backend::setPulse(int n, prf newPulse)
{
    pulses[n] = newPulse;
}

prf backend::getPulse(int pulseNumber)
{
    return pulses[pulseNumber];
}

//Retrieves connection data
int backend::importNetworkSettings(string fileName)
{
    INIReader reader(fileName);

    for(int i=0; i<3; i++)
    {
        // SET UP IP AND PORT

        // Generates strings rhino0, rhino1, rhino2
        string rhinoNo;
        char rhinoChar[8];
        sprintf(rhinoChar, "rhino%d", i);
        rhinoNo = rhinoChar;
        cout << rhinoNo<< "\n";

        backend::rhino[i].ip = reader.Get(rhinoNo,"ip","none");
        backend::rhino[i].port = reader.GetInteger(rhinoNo,"port",-1);

        // DONE SETTING UP IP AND PORT

    }
    // TODO: return error if file does not exist
    return 1;
}


////Imports following parameters from ini file
///
/// Pulse Parameters
///     MB offset
///     DIG offset
///     PRI offset
///     Frequency
///     Mode of operation
///     Not used parameter
/// Start Time
/// End Time
/// Number of unique pulses (N)
/// Number of repeats (M)
int backend::importExperiment(string fileName)
{
    INIReader reader(fileName);
    struct tm tm;   // Used for converting string to time

    // SET UP PULSE PARAMETERS
    for(int j=0; j<32; j++)
    {
        prf pulse;

        string pulseno;
        char pulsechar[8];
        sprintf(pulsechar, "pulse%d", j);
        pulseno = pulsechar;
        cout << pulseno << "\n";

        pulse.mb = reader.GetInteger(pulseno,"MBoffset",1)/10;
        pulse.dig = reader.GetInteger(pulseno,"DIGoffset",1)/10;
        pulse.pri = reader.GetInteger(pulseno,"PRIoffset",1)/10;
        pulse.frequency = reader.GetInteger(pulseno,"Frequency",1);
        pulse.mode = reader.GetInteger(pulseno,"Mode",1);
        pulse.notused = reader.GetInteger(pulseno,"Notused",1);

        setPulse(j, pulse);
    }

    //strptime(reader.Get("settings", "StartTime", "1971-01-01 12:00:00").c_str(), "%d-%m-%Y %H:%M:%S", &tm);
    //mktime(&tm);
    startTime = QDateTime::fromString(reader.Get("settings", "StartTime", "01-01-1971 12:00:00").c_str(), "dd-MM-yyyy HH:mm:ss");
    endTime = QDateTime::fromString(reader.Get("settings", "EndTime", "01-01-1971 12:00:00").c_str(), "dd-MM-yyyy HH:mm:ss");

    n = reader.GetInteger("settings", "UniquePulses", 1);
    m = reader.GetInteger("settings", "Repeats", 1);

    // TODO: return error if file does not exist
    return 1;

}

void backend::rhinoTransmitParameters(int rhinoNumber)
{
    char line[409];

    // Message type
    line[0] = 'P';

    // N
    line[1] = n;

    // M
    line[2] = (m >> 24) & 0xFF;
    line[3] = (m >> 16) & 0xFF;
    line[4] = (m >> 8) & 0xFF;
    line[5] = m & 0xFF;

    // StartTime
    //QDateTime start = QDateTime::currentDateTime();
    //start.setTime(startTime);
    line[6] = (startTime.offsetFromUtc() >> 24) & 0xFF;
    line[7] = (startTime.offsetFromUtc() >> 16) & 0xFF;
    line[8] = (startTime.offsetFromUtc() >> 8) & 0xFF;
    line[9] = startTime.offsetFromUtc() & 0xFF;

    // EndTime
    //QDateTime end = QDateTime::currentDateTime();
    //end.setTime(endTime);
    line[10] = (endTime.offsetFromUtc() >> 24) & 0xFF;
    line[11] = (endTime.offsetFromUtc() >> 16) & 0xFF;
    line[12] = (endTime.offsetFromUtc() >> 8) & 0xFF;
    line[13] = endTime.offsetFromUtc() & 0xFF;

    // Pulse Parameters
    for(int i = 0; i<32; i++)
    {
        string pulseno;
        char pulsechar[8];

        pulseno = pulsechar;

        line[14+i*12] = ((getPulse(i).mb) >> 8) & 0xFF;
        line[15+i*12] = (getPulse(i).mb+1) & 0xFF;

        line[16+i*12] = ((getPulse(i).dig) >> 8) & 0xFF;
        line[17+i*12] = (getPulse(i).dig+1) & 0xFF;

        line[18+i*12] = (getPulse(i).pri >> 8) & 0xFF;
        line[19+i*12] = (getPulse(i).pri+1) & 0xFF;

        // Nybbles are swapped for frequency
        // TODO frequency must have +256. Everything else +1. Figure out why.
        // I know frequency is +256 because nybbles are swapped. need to know why add anything
        line[20+i*12] = (getPulse(i).frequency) & 0xFF;
        line[21+i*12] = ((getPulse(i).frequency+256) >> 8) & 0xFF;

//        line[22+i*12] = (getPulse(i).mode >> 8) & 0xFF;
        line[23+i*12] = (getPulse(i).mode+1) & 0xFF;

        line[24+i*12] = (getPulse(i).notused >> 8) & 0xFF;
        line[25+i*12] = (getPulse(i).notused+1) & 0xFF;
    }

    rhino[rhinoNumber].transmitMessage(line);
}
