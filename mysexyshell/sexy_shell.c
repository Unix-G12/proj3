// Unix Group 12 Project 3 - Linux Sexy Shell
//TODO: handle mycat redirection


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mycat.c"

#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>

#include "include/sexy_shell.h"
#include "myls.c"

#define CYN "\x1B[31m"
#define ORG "\x1b[33m"
#define PNK "\x1b[31m"
#define WHT "\x1b[37m"

int main(int argc, char* *argv) {
	
	// Terminal Background & Foreground Colors
	char a_e_s_t_h_e_t_i_c[64];
	strcpy(a_e_s_t_h_e_t_i_c, "echo -e \"\e[33m\e[40m\"");
	system( a_e_s_t_h_e_t_i_c );
	system("clear");

	int running = 1;
	char *cmd;
	char* *arguments;
	int run_cmd;

	do {
		printf(WHT "$ " ORG);
		cmd = read_cmd();
		cmd_process(cmd, &running);
	} while (running);


	return 1;
}

void reset_colors() {
	char *reset;
	strcpy(reset, "tput init");
	system(reset);
	system("clear");
}
char *read_cmd(void) {
	
	char* line = NULL;
	ssize_t cmd_size = 0;
	getline(&line, &cmd_size, stdin);

	return line;	
}

void cmd_process(char* cmd, int* running) {
	char full_cmd[64];
	strcpy(full_cmd, cmd); 
	char *token;
	char delim[] = "\t\r\n\v\f ";//all possible white spaces
	token = strtok(cmd, delim);//parse command

	if (strcmp(token, "mycd") == 0) {
		//mycd(...);
		printf("mycd..\n");
	}

	else if (strcmp(token, "mycat") == 0){//TODO: loop through all arguments and mycat them
		token = strtok(NULL, delim);//parse next file
		main_cat(token);
		return;	
	}
	else if ((strcmp(token, "exit") == 0 || strcmp(token, "quit") == 0))
	{
		printf("Thanks for shelling!\n");
		reset_colors();
		exit(1);
		running = 0;
	}
	else if (strcmp(token, "mypwd") == 0) {
		pwd();
	}
	else if (strcmp(token, "myls") == 0) {
		main_ls(full_cmd);//e.g. myls -l
	}


	else {
		printf("Error. '%s' not recognized.\n", cmd);
	}
	

	int arg_count = 0;

	while (token != NULL) {
		//printf("%s ", token);
		token = strtok(NULL, delim);
		arg_count++;
	}

	size_t len = 0;
	ssize_t read_line;
}

void pwd() {

	char buffer[4096];
	if(getcwd(buffer, sizeof(buffer)) != NULL){
		printf(WHT "%s", buffer);
		printf("\n");
	}
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
