#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void cd(char** argument) {
  if(argument[1] == NULL)
    printf("Expected argument\n");
  else if(chdir(argument[1]) != 0)
    perror("Error");
}
