#ifndef SHELL_H
#define SHELL_H
#define MAX_CMD_LENGTH 128

struct command {

	int argCount;
	char* *arguments;
};

void pwd();
void mycat();
void mycp();
void myls();
void mycd();

#endif