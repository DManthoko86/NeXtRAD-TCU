/*
	This is the backend header. 

	3) send and receive data from the Spartan 6

*/

// parameters between ARM and FPGA
// _A is on ARM side (temp settings changeable before upload)
// _S is on Spartan side (which are retrieved from spartan and can be updated with an upload)
//
// parameters are often "pointed" by a char. value is in order starting from BCD at 0 and not including suffix
// so for example: BCD is at 0, band is at 1 etc..
// PARAMETER_BITS constant must be 8xthe number of parameters

//#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h> 
#include <stdbool.h>

#define PARAMETER_BITS = 24

struct prf{
	uint16_t mb;
	uint16_t dig;
	uint16_t pri;
	uint16_t frequency;
	uint16_t mode;
	uint16_t notused;
};

unsigned int M = 0;
unsigned int N = 0;
char armed;         // is 1 when Rhino is ready waiting for start bit
char finished;      // us 1 when experiment is over
struct prf reg_prf[32];

// PID of the .bof process. retrieved with getPID()
int pid = 0;

void download(void);

int getPID(char *processName);
char writeToProc(long msg);

unsigned int setM(unsigned int value);
unsigned int setN(unsigned int value);
unsigned int setPRF(int n, uint16_t set_mb, uint16_t set_dig, uint16_t set_pri, uint16_t set_frequency, uint16_t set_mode, uint16_t set_notused);
unsigned int setPRFs(int n, struct prf pulse);
unsigned int uploadPRF(void);

unsigned int getM(void);
unsigned int getN(void);
unsigned int getStatus(void);
struct prf getPRF(int n);

unsigned int startExperiment(void);
