#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

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
