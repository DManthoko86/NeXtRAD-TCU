/*
	Once completed, this program will include functions
	with the following aims

	1) communicate with a user logged on a terminal
	   using smaller "command" programs.
	2) communicate with a user using TCP
	3) send and receive data from the Spartan 6

*/



// contains variables and function declarations
#include "mainsvr.h"

// Changes the number of repeats
unsigned int setM(unsigned int value)
{
	M = value;
	
	char file_dir[30];
	FILE *fp;
	unsigned char buffer[9];
	
	/*	Write to M register	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/m", pid);
	fp = fopen(file_dir, "w");
	
	//puts float in buffer (char*) for fprintf
	buffer[0] = (M & 0xFF);
	buffer[1] = (M >> 8 & 0xFF);
	buffer[2] = (M >> 16 & 0xFF);
	buffer[3] = (M >> 24 & 0xFF);
	
	fwrite(buffer, sizeof(char), 16, fp);
	fclose(fp);
	/*	Done writing to M	*/
	
	return 1;
}

// Changes the number of pulses stored in the Rhino
unsigned int setN(unsigned int value)
{
	
	N = value;	
	
	char file_dir[30];
	FILE *fp;
	unsigned char buffer[9];
	
	/*	Write to N register	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/n", pid);
	fp = fopen(file_dir, "w");
	
	//puts float in buffer (char*) for fprintf
	buffer[0] = (N & 0xFF);
	buffer[1] = (N >> 8 & 0xFF);
	buffer[2] = (N & 0xFF);
	buffer[3] = (N >> 8 & 0xFF);
	
	fwrite(buffer, sizeof(char), 16, fp);
	fclose(fp);
	/*	Done writing to N	*/
	
	return 1;
}

// Tells the Rhino that it can wait for a trigger
unsigned int startExperiment(void)
{
	
	char file_dir[30];
	FILE *fp;
	unsigned char buffer[9];
	
	unsigned int turnon = 1;
	unsigned int turnoff = 0;
	
	stopExperiment();
	
	/*	Write to N register	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_led", pid);
	fp = fopen(file_dir, "w");
	
	//turns on led 1 (start experiment)
	buffer[0] = ((unsigned int)turnon & 0xFF);
	buffer[1] = ((unsigned int)turnon >> 8 & 0xFF);
	buffer[2] = ((unsigned int)turnon >> 16 & 0xFF);
	buffer[3] = ((unsigned int)turnon >> 24 & 0xFF);
	fwrite(buffer, sizeof(char), 16, fp);
	
	fclose(fp);
	
	return 1;
}


// Tells the Rhino that it cant wait for a trigger
unsigned int stopExperiment(void)
{
	
	char file_dir[30];
	FILE *fp;
	unsigned char buffer[9];
	
	/*	Write to N register	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_led", pid);
	fp = fopen(file_dir, "w");
	
	//turns off led register
	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	fwrite(buffer, sizeof(char), 16, fp);
	
	fclose(fp);
	
	return 1;
}

/*
	Writes to the reg_PRF register on the FPGA
	Note that the reg_PRF variable must be written to before
	this function is called. reg_PRF can be written to using
	the setPRF.
*/
unsigned int uploadPRF(void)
{
	char file_dir[30];
	FILE *fp;
	unsigned char buffer[384];
	int i = 0;
	
	// palces float in buffer (char*) for fprintf
	for (i=0; i<32; i++)
	{
		buffer[i*12]   = (reg_prf[i].mb-1) & 0xFF;
		buffer[i*12+1] = (reg_prf[i].mb-1) >> 8 & 0xFF;
		buffer[i*12+2] = (reg_prf[i].dig-1) & 0xFF;
		buffer[i*12+3] = (reg_prf[i].dig-1) >> 8 & 0xFF;
		buffer[i*12+4] = (reg_prf[i].pri-1) & 0xFF;
		buffer[i*12+5] = (reg_prf[i].pri-1) >> 8 & 0xFF;
		buffer[i*12+6] = (reg_prf[i].frequency-1) & 0xFF;
		buffer[i*12+7] = (reg_prf[i].frequency-1) >> 8 & 0xFF;
		buffer[i*12+8] = (reg_prf[i].mode-1) & 0xFF;
		buffer[i*12+9] = (reg_prf[i].mode-1) >> 8 & 0xFF;
		buffer[i*12+10] = (reg_prf[i].notused-1) & 0xFF;
		buffer[i*12+11] = (reg_prf[i].notused-1) >> 8 & 0xFF;
	}
	
	/*	Write from reg_PRF variable to reg_PRF register on FPGA	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_pulses", pid);
	fp = fopen(file_dir, "w");
	
	//if file doesn't exist, return 0
	if (fp == NULL) return(0);
	
	//why is this 254 ??
	fwrite(buffer, 1, 254, fp);
	/*	Done writing to reg_PRF	*/
	
	//otherwise, write message to file
	fclose(fp);
	
	return 1;
}

// Sets the n'th pulse with the parameters pass
//
// setPRF(n, mainbang, dig, pri, frequency, mode, notused)
unsigned int setPRF(int n, uint16_t set_mb, uint16_t set_dig, uint16_t set_pri, uint16_t set_frequency, uint16_t set_mode, uint16_t set_notused)
{
	reg_prf[n].mb 		= set_mb;
	reg_prf[n].dig 		= set_dig;
	reg_prf[n].pri 		= set_pri;
	reg_prf[n].frequency = set_frequency;
	reg_prf[n].mode 	= set_mode;
	reg_prf[n].notused 	= set_notused;
	
	return 1;
}

// setPRF passing the entire struct
unsigned int setPRFs(int n, struct prf pulse)
{
	reg_prf[n] = pulse;
	
	return 1;
}

// Retrieves M (number of repeats) from FPGA register and assigns it to M. Same value is also returned from this function
unsigned int getM(void)
{
	FILE *fp;
	char file_dir[30];

	/*	Retrieve M	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_M", pid);
	fp = fopen(file_dir, "r");
	M	=	fgetc(fp);	// retrieves first character and places in LED_S. there is another 8 bit register, not sure what it is for so it is ignored.
	fclose(fp);
	
	return M;
}

// Retrieves N (number of pulses) from FPGA register and assigns it to N. Same value is also returned from this function
unsigned int getN(void)
{
	FILE *fp;
	char file_dir[30];

	/*	Retrieve M	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_N", pid);
	fp = fopen(file_dir, "r");
	N	=	fgetc(fp);	// retrieves first character and places in LED_S. there is another 8 bit register, not sure what it is for so it is ignored.
	fclose(fp);
	
	return N;
}

// returns the pulse parameters as a prf structure
// note that this fn doesn't return the parameters from the FPGA, but rather the parameters in RAM
// status = 1 : end of experiment
// status = 2 : Rhino waiting for 'start' pulse
// status = 3 : Both of the above
struct prf getPRF(int n)
{
	return reg_prf[n];
}

unsigned int getStatus(void)
{
    FILE *fp;
    FILE *fq;
	char file_dir[30];
	unsigned char isStart, status;

	/*	Retrieve status register	*/
	sprintf(file_dir, "/proc/%i/hw/ioreg/status", pid);
	fp = fopen(file_dir, "r");
	
	// retrieves status register which indicates if it is at the end of an experiment
	status	=	fgetc(fp);
	if(status != 0) status = 1;
	
	fclose(fp);
	
	
	// retrieves from led_reg whether the Rhino is waiting for the 'start' pulse
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_led", pid);
	fq = fopen(file_dir, "r");
	isStart = fgetc(fq);
	if(isStart == 1) 
	{
	    status = status + 2;
	}
	
	fclose(fq);
	return status;
}


// Retrieves PID of the process with the name in the input parameter.
// This would typically be the process of the .bof
int getPID(char *processName)
{

	/* variables for opening and scanning directory */
	DIR *d;
	struct dirent *dir;
	FILE *file;
	char file_dir[80];
	char * line;       //reading the file, important
	size_t len = 0;    //for reading the file. find out if necessary
	ssize_t read;      //also for reading the file. find out if necessary
//	int size_of_line = 0;

	d = opendir("/proc/");

	if (d) 
	{
		while ((dir = readdir(d)) != NULL)
		{
			sprintf(file_dir, "/proc/%s/status", dir->d_name); // stores full directory name in file_dir

			file = fopen(file_dir, "r"); // opens the file located in file_dir

			// detects if status exists. If not, do nothing
			if( file != NULL) 
			{
				read = getline(&line, &len, file);   // reads the first line of the file (hopefully this has the name of the process)

				if(strstr(line, processName) != NULL)
				  pid = atoi(dir->d_name); 
			}
		//fclose(file);     // FIXME! this line gives segmentation error! 
		}
		closedir(d);
	} 
	printf("THE PROCESS ID OF %s IS %i\n", processName, pid);
	return(pid);
}

//Writes to the process
//TODO add error handling
//this is not necessary anymore and has been replaced with upload().
//it is kept here for reference purposes.
char writeToProc(long msg)
{
	FILE *fp;
	char file_dir[30];
	char buffer[5];
	
	//puts float in buffer (char*) for fprintf
	buffer[4] = '\0';
	buffer[0] = (msg & (0xFF));
	buffer[1] = ((msg >> 8) & 0xFF);
	buffer[2] = ((msg >> 16) & 0xFF);
	buffer[3] = ((msg >> 24) & 0xFF);
	
	//directory of the process
	sprintf(file_dir, "/proc/%i/hw/ioreg/reg_led", pid);
	
	fp = fopen(file_dir, "w");
	
	//if file doesn't exist, return 0
	if (fp == NULL) return(0);
	
	//otherwise, write message to file
	fprintf(fp, "%s", buffer);
	fclose(fp);
	return(1);
}
