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
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>


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
/*******************************************************/

void pwd() {

	char buffer[4096];
	if(getcwd(buffer, sizeof(buffer)) != NULL){
		printf(WHT "%s", buffer);
		printf("\n");
	}
}

/*******************************************************/
//mycp
/*******************************************************/
/* Check if its a directory or a file */

int type(const char *path){   
    struct stat bf;   
    if(stat(path, &bf) == -1){ 
        printf("dir(%s), stat(%s) error!\n",path, path);   
          return 0;   
    }   
    if((S_IFMT & bf.st_mode) == S_IFDIR) {   
        return 1;   
    }   
    else   
        return 0;   
}
int copy(const char *src, const char *dest) {
	int fin, fout, count;
	char buffer[BUFSIZ];
	struct stat old_mode;
	if(stat(src, &old_mode) == -1){ 
		printf("cp: (%s) : No such file or directory!\n",src);   
		return 0;   
    }

    /* Open input and output files */
    if( (fin = open(src, O_RDONLY)) == -1){   
        printf("cp:(%s, %s), stat(%s) error!\n", src, dest, src);   
		return 0;
	}
    if((fout = open(dest, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
        printf("cp:(%s, %s), can't open %s.\n", src, dest, src);   
		close(fin);
		return 0;
	}

	 /* Transfer data until we encounter end of input or an error */
	//count = read(fin, buffer, BUFSIZ);
	while((count = read(fin, buffer, BUFSIZ)) > 0){   
		if(write(fout, buffer, count) != count) {
			printf("CAN NOT WRITE TO DESTINATION\n");
			close(fin);
			close(fout);
			return 0;
		}
	}
	close(fin);
	close(fout);
	return 1;
}
/* Function to get the name of the file or directory */
void getName(char *bf, char *name)   
{   
    int i,n, j;   
    n = strlen(bf);  
    for(i = n - 1; i >=0 ; i--){   
        if(bf[i]=='/'){   
            break;   
        }   
    }   
    for(i++, j = 0; i < n; i++, j++)   
        name[j] = bf[i];   
    name[j] = '\0';   
}
void recursion(const char *src, const char *dest){
	char bffsrc[BUFSIZ], bffdest[BUFSIZ], name[BUFSIZ]; 
	//int flag = type(src); /* Create a flag to check directory/file */
	strcpy(bffsrc, src);   /* Copy */
   	strcpy(bffdest, dest);  /* Copy  */
    
    /* If it's a regular file */
 if (S_ISREG(mode.st_mode)){
	getName(bffsrc, name); /* Get the file name */
    	strcat(bffdest, "/");
     	strcat(bffdest, name);    /* Append src to the end of dest */
     	copy(bffsrc, bffdest);   
     	return;
     }
     /* If it's a directory */
 else if (S_ISDIR(mode.st_mode)){ 
	getName(bffsrc, name); /* Get directory name*/
        strcat(bffdest, "/");  
        strcat(bffdest, name); /* Append src to the end of dest */

        if(strcmp(name, ".") ==0 || strcmp(name, "..") ==0 ){ 
        	return;
        }
    	
    	struct stat old_mode; 

        if(stat(src, &old_mode) == -1){
        	printf("Error!\n");   
        	return;  
        }
   
        mkdir(bffdest, old_mode.st_mode); /* Create destination dir */
        chmod(bffdest, old_mode.st_mode); /* Change mode of destination dir */

        DIR * pdir = opendir(bffsrc);   
        struct dirent * pdirent;   

        while(1){
        	pdirent = readdir(pdir);
        	if(pdirent == NULL){
        		break;
        	}
        	else{
        		strcpy(bffsrc, src); /* Copy files */
                strcat(bffsrc, "/");   
                strcat(bffsrc, pdirent->d_name);    
                mkdir(dest, old_mode.st_mode); 	/* Create directory if it doesnt exist */
                mkdir(bffdest, old_mode.st_mode); 
        	//chmod(bffdest, old_mode.st_mode); /*change mode of bfto  */
                recursion(bffsrc, bffdest); 
        	}
        }
        closedir(pdir);
        return;
    }
    else
    	return;
}
int executecp(int argc, char*argv[]){    
    char *src, *dest;

    if(argc <3){
    	printf("File Usage: %s source destination\n", argv[0]);
    	printf("Recursive Usage: %s -R source destination\n", argv[0]);
    	return 0;
    }
    if(strncmp(argv[1], "-R", 5) != 0 && argc >4 ){
    	printf("Usage: %s -R source destination\n", argv[0]);
    	return 0;
    }
    if(strncmp(argv[1], "-R", 5) == 0){
    	src = argv[2];
        dest = argv[3];
        recursion(src, dest);
    }
    else{
        src = argv[1];
        dest = argv[2];
        copy(src, dest);
    }
    return 0;
}
/*******************************************************/

void myls() {

}

/*******************************************************/
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
