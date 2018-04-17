#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mycd(char *argument); //function declaration

int main(int argc, char **argv) {
  char *argument; //not assigned unless argument exists

  if(argc != 2) { //check for argument existence
    printf("Usage: mycd <directory>\n");
    exit(1);
  }

  argument = argv[1]; //assign to command line argument
  mycd(argument); //change directory
  exit(0);
}

void mycd(char *argument) {
  char *currentPath = getenv("PATH"); //path user is in
  printf("Path: %s\n", currentPath); //comparitive check
  char *delimiterCheck = strstr(argument, "/"); //check if delimiter exists
  char *tempPath; //temporary path holder
  char *token = strtok(argument, "/"); //delimiter for argument path
  int count = 0; //generic count
  int oldCount = 0; //count for old path
  int size = 0; //size of path
  int i = 0; //generic count
  int j = 0; //generic count
  char **newPath = (char **) malloc(sizeof(char *) * (count + 1)); //allocate size for argument
  char **oldPath = (char **) malloc(sizeof(char *) * (oldCount + 1));  //allocate size for old path

  if(delimiterCheck != NULL) { //if argument has the delimiter
    while(token != NULL) { //while delimiter still exists
      newPath[count] = malloc(sizeof(char) * ((strlen(token) + 1))); //allocate size for delimiter string
      strcpy(newPath[count], token); //copy string into array
      printf("%s\n", newPath[count]); //array check
      count++;
      token = strtok(NULL, "/"); //continue to next delimiter

      if(token != NULL) { //if delimiter still exists
        char **resize = (char **) realloc(newPath, sizeof(char *) * (count + 1)); //resize array
      
        if(resize != NULL) //if resizing does not fail
          newPath = resize; //path array becomes size of resize
      }
    }
  }
  else { //if argument does not have delimiter
    newPath[count] = argument;
    printf("%s\n", newPath[count]); //array check
    count++;
  }  

  while(i < count) { //while going through array
    if(strcmp(newPath[i], ".") == 0) { //if argument is .
      if(setenv("NEWPATH", currentPath, 0) == -1) { //check that setting environment does not fail
        perror("Error");
        exit(1);
      }
    currentPath = getenv("NEWPATH"); //make current path new path
    printf("New Path: %s\n", currentPath); //check path is correct
    }
    else if(strcmp(newPath[i], "..") == 0) { //if path is ..
      delimiterCheck = strstr(currentPath, "/"); //check if there is a delimiter
      token = strtok(currentPath, "/");

      if(delimiterCheck != NULL) { //if there is a delimiter
        while(token != NULL) { //while delimiter still exists
          oldPath[oldCount] = malloc(sizeof(char) * ((strlen(token) + 1))); //allocate size for array
          size = size + strlen(token) + 1; //keep size of path
          strcpy(oldPath[oldCount], token);
          oldCount++;
          token = strtok(NULL, "/");

          if(token != NULL) { //if delimiter still exists
            char **resize = (char **) realloc(oldPath, sizeof(char *) * (oldCount + 1)); //resize array

            if(resize != NULL) //if resize does not fail
              oldPath = resize; //resize
          } 
        }
printf("Before temp alloccation\n"); 
        tempPath = malloc(size); //allocate temporary array to size of old path
printf("Size: %d\n", sizeof(tempPath));
        tempPath = "";
printf("Before loop\n");
        while(j < oldCount - 1) {
          strncat(tempPath, oldPath[j], strlen(oldPath[j])); //ERROR: Segmentation fault
printf("In loop %d\n", j);
printf("String: %s\n", oldPath[j]);
          if(j < oldCount - 2)
            strcat(tempPath, "/"); //add delimiter if not at the end
          j++;
        }
        oldCount--;
        currentPath = tempPath; //make temporary path new path
      
        if(setenv("NEWPATH", currentPath, 1) == -1) { //check if setting environment fails
          perror("Error");
          exit(1);
        }   
      }
      else { //keep path the same if at top directory
        if(setenv("NEWPATH", currentPath, 0) == -1) {
          perror("Error");
          exit(1);
        }
        currentPath = getenv("NEWPATH"); //get new path
        printf("New Path %s\n", currentPath); //check if path is correct
      }
    }
    i++;
  }
}
