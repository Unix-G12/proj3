/*
-R If source_file designates a directory, cp copies the directory and the entire subtree 
connected at that point. If the source_file] ends in a /, the contents of the directory 
are copied rather than the directory itself. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> // For struct status
#include <fcntl.h>
#include <unistd.h> //Header needed to use system call
#include<dirent.h> // closedir, DIR, opendir

//#define BUFFER 1024 , using BUFSIZ instead

/* Function to get the name of the file or directory */
void getName(char *bff, char *name)   
{   
    int i, j;   
    int n = strlen(bff);  
    for(i = n - 1; i >=0 ; i--){   
        if(bff[i]=='/'){   
            break;   
        }   
    }   
    for(i++, j = 0; i < n; i++, j++)   
        name[j] = bff[i];   
    name[j] = '\0';   
} 

/* Check if its a directory or a file */
int dir(const char *path)   
{   
    struct stat bff;   
    if(stat(path, &bff) == -1){   
        printf("error!");   
        return -1;   
    }   
    if((S_IFMT & bff.st_mode) == S_IFDIR) {   
        return 1;   
    }   
    else   
        return 0;   
}   

int copy(char *src, char *dest) {
		char buffer[BUFSIZ];
	int fin, fout, charCount;
	struct stat mode;

    if(stat(src, &mode) == -1){   
        printf("error!\n");   
        return 0;   
    }

    if( (fin = open(src, O_RDONLY)) == -1){   //open and set to read only 
		printf("SOURCE FILE CAN NOT BE OPENED\n");
		return 0;
	}
	

    if( (fout = creat(dest, mode.st_mode)) == -1){    //open and set to read only, creates if file DNE
		printf("DESTINATION FILE CAN NOT BE OPENED\n");
		close(fin);
		return 0;
	}	

	if(fchmod(fout, mode.st_mode) == -1){   
        printf("copy(%s, %s), fchmod(%s) error!\n", src, dest, dest);   
        return 0;   
    }   
	/* Read and write */
	charCount = read(fin, buffer, BUFSIZ);
	while(charCount > 0) { //copies the directory or file
		if(write(fout, buffer, charCount) != charCount) {
			printf("CAN NOT WRITE TO DESTINATION\n");
			close(fin);
			close(fout);
			return 0;
		}
	}
	close(fin);
	close(fout);
	return 1;
	
	if(close(fin) == -1) {
		printf("CAN NOT CLOSE SOURCE\n");
		exit(EXIT_FAILURE);		
	}
	if(close(fout) == -1) {
		printf("CAN NOT CLOSE DESTINATION\n");
		exit(EXIT_FAILURE);			
	}
	return 1;	
	
	/********************** Original Code:********************
	char buffer[BUFSIZ];
	int fin, fout, charCount;
	//struct stat old_mode;
	//int fout;
	//int charCount;
	//int hold;
	
	fin = open(src, O_RDONLY);  //open and set to read only
	if(fin == -1) {
		printf("SOURCE FILE CAN NOT BE OPENED\n");
		exit(EXIT_FAILURE);
	}
	
	fout = open(dest, O_WRONLY | O_CREAT); //open and set to read only, creates if file DNE
	if(fout == -1) {
		printf("DESTINATION FILE CAN NOT BE OPENED\n");
		exit(EXIT_FAILURE);
	}	
	
	charCount = read(fin, buffer, BUFSIZ);
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
	*******************************************************/
}

void recursion(const char *src, const char *dest){
	char bffsrc[BUFSIZ]; /* Used to store the filepath of source */
	char bffdest[BUFSIZ]; /* Used to store the filepath of destincation */
	char name[BUFSIZ]; /* Used  for filename */
	struct stat mode; 

	int flag = dir(src); /* Create a flag to check directory/file */

	strcpy(bffsrc, src);   /* Copy */
    strcpy(bffdest, dest);  /* Copy  */

     if(flag == 0){ /* If flag === file */
     	getName(bffsrc, name);  /* Get the file name */   
     	strcat(bffdest, "/"); /* Append src to dest */
     	strcat(bffdest, name);    /* Append src to the end of dest */
     	copy(bffsrc, bffdest);   
     	return;
     }
     else if(flag == 1){
     	getName(bffsrc, name); /* Get directory name*/
        strcat(bffdest, "/");  /* concatenates bffdest and "/" and stores result in bffdest*/
        strcat(bffdest, name); /* concatenates bffdest and "name" and stores result in bffdest*/

        if(strcmp(name, ".") ==0 || strcmp(name, "..") ==0 ){ /* Compares the strings */
        	return;
        }
        if(stat(src, &mode) == -1){
        	printf("error!\n");   
        	return;  
        }
        mkdir(bffdest, mode.st_mode); 
        chmod(bffdest, mode.st_mode); 
	     
	/* Magic */
        DIR * pdir =  opendir(bffsrc);   
        struct dirent * pdirent;   

        while(1){
        	pdirent = readdir(pdir);
        	if(pdirent == NULL){
        		break;
        	}
        	else{
        		strcpy(bffsrc, src);
                strcat(bffsrc, "/");   
                strcat(bffsrc, pdirent->d_name);   
                copy(bffsrc, bffsrc); 
        	}
        }
        closedir(pdir);
        return;
    }
    else{
    	return;
    }
}

int main(int argc, char *argv[]) {
    if(argc != 3) {   
        printf("usage: source destination \n ", argv[0]);   
        return -1;   
    }   
    recursion(argv[1], argv[2]);  
	return 0;			
	
	/************** Original Code ************************
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
			//recursion(src, dest);
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
	**************************************/	
}
