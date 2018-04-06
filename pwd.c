#include <stdio.h>
#include <unistd.h>

int pwd(int argc, char** argv) {
	char buffer[4096];
	if(getcwd(buffer, sizeof(buffer)) != NULL){
		printf("%s\n", buffer);
	}
	return 0;
}
