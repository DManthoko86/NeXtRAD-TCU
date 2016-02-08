/*
   server.cpp

   Test server for the tcpsockets classes which handles connections
   iteratively.

   ------------------------------------------

   Copyright © 2013 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   
   Example code edited by Johann Burger to suit the NeXtRAD project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tcpsocket/tcpacceptor.h"
#include "mainsvr/mainsvr.c"
//#include "inih/ini.c"

// functions
int uploadAllParam(char* line);
int launchBitStream(void);
void getAllParam(char * line);
void sendStatus();


unsigned int StartTime, EndTime;
    
int main(int argc, char** argv)
{
    char line[20];
    char character;
    
	if (argc < 2 || argc > 4) {
        printf("usage: server <port> [<ip>]\n");
        exit(1);
    }
    
    // Starts the TCU gateware. 
    launchBitStream();

    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (argc == 3) 
    {
        acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else 
    {
        acceptor = new TCPAcceptor(atoi(argv[1]));
    }
    
    if (acceptor->start() == 0) 
    {
    	// Waiting for message from client
        while (1) 
        {
       		// Retrieved message from client
            stream = acceptor->accept();
            if (stream != NULL) 
            {
                ssize_t len;
                char line[410];
                while ((len = stream->receive(line, sizeof(line))) > 0) 
                {
                	// This is what happens when the Server retrieves a message.
                	
                    line[len] = 0;
                    printf("received - %s\n", line);
//                    stream->send(line, len);
                    
                    if(line[0] == 'P')
                    {
                    	if(uploadAllParam(line) == 1) 
                    	    stream->send("All experiment parameters uploaded successfully", 64);
                    }
                    // Are you alive?
                    else if (line[0] == 'A')
                    {
                        stream->send("Yes, i'm still alive", 21);
                    }
                    // return all the parameters
                    else if(line[0] == 'B')
                    {
                        getAllParam(line);
                    }
                    else if(line[0] == 'S')
                    {
                        line[0] = 'S';
                        line[1] = getStatus();
//                        sprintf(line, "S%c - status", 'd');//character);
                        stream->send(line, 5);
                    }
                }
                delete stream;
            }
        }
    }
    exit(0);
}



int uploadAllParam(char* line)
{
    // Second byte is N
    bool status = setN(line[1]);
    // Third to sixth bytes are M
    status = status & setM((line[2] << 24) + (line[3] << 16) + (line[4] << 8) + line[5]);
    StartTime = (line[6] << 24) + (line[7] << 16) + (line[8] << 8) + line[9];
    EndTime = (line[10] << 24) + (line[11] << 16) + (line[12] << 8) + line[13];
    
    struct prf pulse;
    
    for(int i = 0; i<32; i++)
    {
        pulse.mb        = (line[14+i*12] << 8) + line[15+i*12];
        pulse.dig       = (line[16+i*12] << 8) + line[17+i*12];
        pulse.pri       = (line[18+i*12] << 8) + line[19+i*12];
        pulse.frequency = (line[20+i*12] << 8) + line[21+i*12];
        pulse.mode      = (line[22+i*12] << 8) + line[23+i*12];
        pulse.notused   = (line[24+i*12] << 8) + line[25+i*12];
        
        setPRFs(i, pulse);
    }
    
    uploadPRF();
    
    return status;
}

// Launches gateware and returns the PID
int launchBitStream(void)
{
    // Launches gateware
	// waits for gateware bitstream to finish (pessimistic wait function)
	// gets PID
	system("./TCU.bof &");
	usleep(3000000);
	
	printf("Retrieving PID.\n");
	
	int pid = getPID("TCU.bof");	//retrieves PID and stores it in the global int pid
	printf("\n");
	
	return pid;
}



void getAllParam(char * line)
{
    struct prf pulse;
    
    // Message type
    line[0] = 'P';

    // N
    line[1] = getN();

    // M
    line[2] = (getM() >> 24) & 0xFF;
    line[3] = (getM() >> 16) & 0xFF;
    line[4] = (getM() >> 8) & 0xFF;
    line[5] = getM() & 0xFF;

    // StartTime
    line[6] = (StartTime >> 24) & 0xFF;
    line[7] = (StartTime >> 16) & 0xFF;
    line[8] = (StartTime >> 8) & 0xFF;
    line[9] = StartTime & 0xFF;

    // EndTime
    line[10] = (EndTime >> 24) & 0xFF;
    line[11] = (EndTime >> 16) & 0xFF;
    line[12] = (EndTime >> 8) & 0xFF;
    line[13] = EndTime & 0xFF;

    // Pulse Parameters
    for(int i = 0; i<32; i++)
    {
        pulse = getPRF(i);
        
        line[14+i*12] = (pulse.mb >> 8) & 0xFF;
        line[15+i*12] = (pulse.mb) & 0xFF;
        
        line[16+i*12] = (pulse.dig >> 8) & 0xFF;
        line[17+i*12] = (pulse.dig) & 0xFF;
        
        line[18+i*12] = (pulse.pri >> 8) & 0xFF;
        line[19+i*12] = (pulse.pri) & 0xFF;
        
        line[20+i*12] = (pulse.frequency >> 8) & 0xFF;
        line[21+i*12] = (pulse.frequency) & 0xFF;
        
        line[22+i*12] = (pulse.mode >> 8) & 0xFF;
        line[23+i*12] = (pulse.mode) & 0xFF;
        
        line[24+i*12] = (pulse.notused >> 8) & 0xFF;
        line[25+i*12] = (pulse.notused) & 0xFF;
    }
    
}

