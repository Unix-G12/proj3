#include<sys/stat.h>   
#include<sys/types.h>   
#include<unistd.h>   
#include<fcntl.h>   
#include<dirent.h>   
#include<stdio.h>   
#include<string.h>   

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
	int flag = type(src); /* Create a flag to check directory/file */
	strcpy(bffsrc, src);   /* Copy */
    strcpy(bffdest, dest);  /* Copy  */
    
    /* If it's a regular file */
    if(flag == 0){
    	getName(bffsrc, name); /* Get the file name */
    	strcat(bffdest, "/");
     	strcat(bffdest, name);    /* Append src to the end of dest */
     	copy(bffsrc, bffdest);   
     	return;
     }
     /* If it's a directory */
     else if(flag == 1){
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

int main(int argc, char *argv[]) {
    return executecp(argc, argv);

}
