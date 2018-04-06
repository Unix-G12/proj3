//Unix Group 12 Project 3 - Linux Sexy Shell


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/sexy_shell.h"

int main(int argc, char* *argv) {
	
	char *cmd = (char *)malloc(MAX_CMD_LENGTH * sizeof(char));

	for (int i = 0; i < argc; i++) {
		strcat(cmd, argv[i]);
		cmd[strlen(cmd)] = ' ';
	}

	char *token;
	const char delim[2] = " ";
	token = strtok(cmd, delim);

	while (token != NULL) {
		printf("%s ", token);
		token = strtok(NULL, delim);
	}
	printf("\n");

	return 1;
}