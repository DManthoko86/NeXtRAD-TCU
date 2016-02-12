/*
	Once completed, this program will act as a backend server
	with the following aims

	1) communicate with a user logged on a terminal
	   using smaller "command" programs.
	2) communicate with a user using TCP
	3) send and receive data from the Spartan 6

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

// contains variables and function declarations
#include "CLI.h"
#include "mainsvr.c"


int main(int argc, char *argv[])
{
	char *menuItem = "\0";
	char message[30];
	struct paramMain main;
	struct paramCLI cli;
	
	
	int i = 0;
	
	char *paramName;
	char *value;
	
	//dispSplash();
	
	printf("\nWelcome to the TCU CLI for the rhino!\n\n");
	printf("Uploading bitstream.\n");
	
	// Launches gateware
	// waits for gateware bitstream to finish (pessimistic wait function)
	// gets PID
	system("./TCU.bof &");
	usleep(2700000);
	
	printf("Retrieving PID.\n");
	getPID("TCU.bof");	//retrieves PID and stores it in the global int pid
	printf("\n");
	dispHelp();
	
	for(;;)
	{
	
	
		printf(">> ");
		scanf("%s", command);
		
		if(strcmp(command,"exit") == 0 || strcmp(command,"quit") == 0 || strcmp(command,"q") == 0 || strcmp(command,"e") == 0)
		{
			printf("Killing bof process.\n");
			
			// Kills bof process
			sprintf(message, "kill %i", pid);
			system(message);
			
			printf("goodbye.\n");
			return 1;
		}
		else if(strcmp(command,"display") == 0 || strcmp(command,"d") == 0)
		{
			
		}
		else if(strcmp(command,"n") == 0)
		{
			printf("\nEnter the number of pulses (N): ");
			scanf("%i",&i);
			setN(i);
			i=0;
		}
		else if(strcmp(command,"m") == 0)
		{
			printf("\nEnter the number of repeats (M): ");
			scanf("%i",&i);
			setM(i);
			i=0;
		}
		else if(strcmp(command,"getpulse") == 0 || strcmp(command,"g") == 0)
		{
			printf("MB offset\tDig offset\tPRI offset\tFrequency\tMode\t\tUnused");
			
			for(i=0;i<32;i++)
				getPulse(i);
				
			printf("\n");
		}
		else if(strcmp(command,"setpulse") == 0 || strcmp(command,"s") == 0)
		{
			setPulse();
		}
		else if(strcmp(command,"h") == 0 || strcmp(command,"help") == 0)
			dispHelp();
		else if(strcmp(command,"download") == 0 || strcmp(command, "l")==0);
			
		else if(strcmp(command,"upload") == 0 || strcmp(command, "u")==0)
			uploadPRF();
		else if(strcmp(command,"go") == 0 || strcmp(command, "g")==0)
		{
			startExperiment();
		}
	}
	
  return 0;
}

void dispSplash(void)
{
	printf("Welcome to the TCU CLI for the rhino!\n\n");
	dispHelp();
}

void dispHelp(void)
{
	printf("Below is a list of the available commands:\n");
	printf("------------------------------------------\n");
	printf("(h)elp\t\tdisplays this screen.\n");
	printf("(d)isplay\t\tdusplays all parameters\n");
	printf("(s)etpulse\t\tchange pulse parameter.\n");
	printf("Change (n)\t\tchange number of pulses N.\n");
	printf("Change (m)\t\tchange number of repeats M.\n");
	printf("(g)etpulse\t\tdisplay a pulse's parameters.\n");		// replaced with display
//	printf("down(l)oad\t\tquery for parameter values in FPGA\n");
	printf("(u)pload\t\tpush pulse parameter values to FPGA\n");
	printf("(g)o\t\tstart experiment.");
	printf("(e)xit(q)\t\tquits the program.\n\n");
}


void setPulse()
{
	int i = 0;
	bool flag = false;
	struct prf pulse;
	
	while(flag == false)
	{
		printf("\nEnter pulse number: ");
		scanf("%d",&i);
		if(i<32 && i>=0) flag = true;
		else printf("Pulse number must be an integer between 0 and 31");
	}
	
	printf("\nEnter Main Bang offset in 10s of nanoseconds: ");
	scanf("%" SCNi16 ,&pulse.mb);
	
	printf("\nEnter Digitisation offset in 10s of nanoseconds: ");
	scanf("%" SCNi16 ,&pulse.dig);
	
	printf("\nEnter Next PRI offset in 10s of nanoseconds: ");
	scanf("%" SCNi16 ,&pulse.pri);
	
	printf("\nEnter frequency in mHz: ");
	scanf("%"  SCNi16 ,&pulse.frequency);

	printf("\nMode of Operation\tFreq band\tTx Pol\tRx Pol");			
	printf("\n0\t\t\tL\tVertical\tVertical");
	printf("\n1\t\t\tL\tVertical\tHorizontal");
	printf("\n2\t\t\tL\tHorizontal\tVertical");
	printf("\n3\t\t\tL\tHorizontal\tHorizontal");
	printf("\n4\t\t\tX\tVertical\tHorizontal,Vertical");
	printf("\n5\t\t\tX\tHorizontal\tHorizontal,Vertical");
	printf("\nEnter Mode of Operation: ");
	scanf("%" SCNi16 ,&pulse.mode);
	
	printf("\nEnter the value of the unused parameter: ");
	scanf("%" SCNi16 ,&pulse.notused);
	
	/*
	printf("\nEnter parameter you wish to change: ");
	scanf("%s",cli.paramName);
	printf("Enter value you wish to change %s to: ",cli.paramName);
	scanf("%s",cli.value);
	
	// sets parameter (within CLI)
	setPar(cli, message);
	printf("%s\n", message);
	*/
	
	setPRFs(i, pulse);
}

void getPulse(int n)
{
	printf("\n");
	printf("%" PRIi16 , reg_prf[n].mb);
	printf("\t\t%" PRIi16 , reg_prf[n].dig);
	printf("\t\t%" PRIi16 , reg_prf[n].pri);
	printf("\t\t%" PRIi16 , reg_prf[n].frequency);
	printf("\t\t%" PRIi16 , reg_prf[n].mode);
	printf("\t\t%" PRIi16 , reg_prf[n].notused);
	
}



//converts parameter string to parameter char for use in mainsvr.c
//also changes value accordingly
struct paramMain paramCLIToMain(struct paramCLI cli)
{
	struct paramMain toReturn;
	
	if( strcmp(cli.paramName,"BCD") == 0 || strcmp(cli.paramName,"bcd") == 0 ) {
		toReturn.paramName = 0;
		toReturn.value = atoi(cli.value);
	}
	else if( strcmp(cli.paramName,"band") == 0) 
	{
		toReturn.paramName = 1;
		if (strcmp(cli.value,"L")==0 || strcmp(cli.value,"l")==0) toReturn.value = 0;
		else  if (strcmp(cli.value,"X")==0 || strcmp(cli.value,"x")==0) toReturn.value = 1;
	}
	else if(strcmp(cli.paramName,"LED") == 0 || strcmp(cli.paramName,"led") == 0)
	{
		toReturn.paramName = 2;
		toReturn.value = atoi(cli.value);
	}
	else
	{
		toReturn.paramName = 255;
		toReturn.value = 0;
	}
	
	return(toReturn);
}
