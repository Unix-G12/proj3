#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stats.h>
#include <fcntl.h>
#include <unistd.h> //Header needed to use system call
#define BUFFER 1024

int copy(char *src, char *dest) {
	
	char buffer[BUFFER];
	int fin, fout, charCount;
	//int fout;
	//int charCount;
	int hold;
	
	fin = open(src, O_RDONLY);  //open and set to read only
	if(fin == -1) {
		printf("SOURCE FILE CAN NOT BE OPENED\n");
		exit(EXIT_FAILURE);
	}
	
	fout = open(dest, O_WRONLY | O_CREATE); //open and set to read only, creates if file DNE
	if(fout == -1) {
		printf("DESTINATION FILE CAN NOT BE OPENED\n");
		exit(EXIT_FAILURE);
	}	
	
	charCount = read(fin, buffer, BUFFER);
	while(charCount > 0) { //copies the directory or file
		if(write(fout, buffer, charCount) != charCount) {
			printf("CAN NOT WRTIE TO DESTINATION\n");
			exit(EXIT_FAILURE);
		}
		if(charCount == -1) {
			printf("CAN NOT READ FROM SOURCE\n");
			exit(EXIT_FAILURE);			
		}
	}
	
	if(close(fin) == -1) {
		printf("CAN NOT CLOSE SOURCE\n");
		exit(EXIT_FAILURE);		
	}
	if(close(fout) == -1) {
		printf("CAN NOT CLOSE DESTINATION\n");
		exit(EXIT_FAILURE);			
	}
	return 1;
}
/*
void recursion(const char *src, const char *dest) {
	
	
}
*/

int main(int argc, char *argv[]) {
		
	char *src;
	char *dest;
		
	if(strncmp(argv[1], "-R", 5) != 0) { //checks if there are too many args for recursion
		if(argc > 4) {
			printf("TOO MANY ARGUEMENTS CALLED FOR -R\n");
			exit(EXIT_FAILURE);		
		}
	}
	else if(strncmp(argv[1], "-R", 5) == 0) {
		if(strcmp(argv[2], argv[3]) == 0) { //checks if src and dest are same
			printf("SOURCE AND DESTINATION ARE THE SAME FILE\n");
			printf("DIRECTORY CAN NOT BE COPIED\n");
			exit(EXIT_FAILURE);
		}
		else {
			src = argv[2];
			dest = argv[3];
			recursion(src, dest);
			printf("DIRECTORY COPIED\n");
		}	
	}
	else {
		if(strcmp(argv[1], argv[2]) == 0) { //checks if src and dest are same
			printf("SOURCE AND DESTINATION ARE THE SAME FILE\n");
			printf("FILE CAN NOT BE COPIED\n");
			exit(EXIT_FAILURE);
		}
		else {
			src = argv[1];
			dest = argv[2];
			copy(src, dest);
			printf("FILE COPIED\n");
		}
	}
	return 0;	
}

