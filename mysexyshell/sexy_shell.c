// Unix Group 12 Project 3 - Linux Sexy Shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void pwd() {

	char buffer[4096];
	if(getcwd(buffer, sizeof(buffer)) != NULL){
		printf("%s\n", buffer);
	}
}

void mycat() {

}

void mycp() {

}

void myls() {

}

void mycd(char* argument) {
  if(argument == NULL) {
    printf("Expected argument\n");
    exit(1);
  }
  
  const int MAX = 4096;
  char path[MAX];
  char cwd[MAX + 1];

  strcpy(path, argument);

  if(path[0] != '/') {
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/");
    strcat(cwd, path);
    chdir(cwd);
  }
  else
    chdir(path);
  exit(0);
}
