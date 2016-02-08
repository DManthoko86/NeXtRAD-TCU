#define MAX_COMMAND_SIZE 6
#define MAX_OPT1_SIZE 6

//TODO size must be set. spits segmentation fault otherwise.
//parameter name and value from the mainsvr side. Each parameter is given a char code and value is an int
struct paramMain{
	char paramName;
	unsigned int value;
};

//parameter name and value from the CLI side. Parameter name is a string and value is either an int or a string/character.
struct paramCLI{
	char paramName[15];
	char value[5];
};

char command[10];
char opt1[10];

void dispSplash(void);
void dispHelp(void);
char setPar(struct paramCLI cli, char *message);
//struct paramCLI paramMainToCLI(struct paramMain);
struct paramMain paramCLIToMain(struct paramCLI);

void setPulse(void);

void getPulse(int n);
