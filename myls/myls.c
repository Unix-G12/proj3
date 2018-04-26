#include <dirent.h> 
#include <time.h> 
#include <sysexits.h> 
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdbool.h> 
#include <pwd.h> 
#include <grp.h> 
#include <string.h> 
#include <getopt.h> 
#include <stdio.h>
#include <stdlib.h>

bool opt_l = false, opt_a = false;

struct stat getStats(const char *file, char *dir);
void printStats(char *dir, char *file);
static int compareLex(const void *p1, const void *p2);

int main_ls(char* cmd){

	/*int opt;
	while ((opt = getopt(argc, argv, "la")) != -1) {
		switch (opt) {
			case 'l':
				opt_l = true; 
				break;
			case 'a':
				opt_a = true;
				break;
			case '?':
				perror("At least one option is not a valid option for myls in Sexy Shell ;)\n");
				return -1;
				//exit(EX_USAGE);
		}
	}*/
	if (strstr(cmd, "-l")){
		opt_l = true;
	}


	char *dir = ".";
	DIR *dirPtr = opendir(dir);
	struct dirent *direntPtr = readdir(dirPtr);
	char **dirArray = malloc(20000*sizeof(char*));
	int count =0;
	while(direntPtr){
		if(opt_a==false && direntPtr->d_name[0]=='.'){ 
			direntPtr = readdir(dirPtr);
			continue; }
		dirArray[count] = direntPtr->d_name;
		count++;
		direntPtr = readdir(dirPtr);
	}
	qsort(dirArray, count, sizeof(char*), compareLex);
	if(!opt_l){
		int i;
		for(i = 0; i < count; i++){
		printf("%s\n", dirArray[i]);
		}
	}
	else{
		int i;
		for (i = 0; i < count; i++) {
		printStats(dir, dirArray[i]);
		}
	}
	closedir(dirPtr);
	free(dirArray);
}

//Alphabetically sort output
static int compareLex(const void *p1, const void *p2) {
	const char *str1 = *(const void **)p1;
	const char *str2 = *(const void **)p2;
	return strcasecmp(str1, str2);
}

struct stat getStats(const char *file, char *dir) {
	char path[1024];
	sprintf(path, "%s/%s", dir, file);
	struct stat sb;
	if (stat(path, &sb) < 0) {
		perror("Error in getstats\n");
		exit(EX_IOERR);
	}
	return sb;
}

void printStats(char *dir, char *file) {
	
	struct stat sb = getStats(file, dir);
	if(sb.st_mode == S_IFREG){ putchar('-');}
	else if(sb.st_mode == S_IFDIR){ putchar('d');}
	putchar((sb.st_mode & S_IRUSR) ? 'r' : '-');
	putchar((sb.st_mode & S_IWUSR) ? 'w' : '-');
	putchar((sb.st_mode & S_IXUSR) ? 'x' : '-');
	putchar((sb.st_mode & S_IRGRP) ? 'r' : '-');
	putchar((sb.st_mode & S_IWGRP) ? 'w' : '-');
	putchar((sb.st_mode & S_IXGRP) ? 'x' : '-');
	putchar((sb.st_mode & S_IROTH) ? 'r' : '-');
	putchar((sb.st_mode & S_IWOTH) ? 'w' : '-');
	putchar((sb.st_mode & S_IXOTH) ? 'x' : '-');
	printf("%5ld", sb.st_nlink);
	printf("%6s ", getpwuid(sb.st_uid)->pw_name);
	printf("%6s ", getgrgid(sb.st_gid)->gr_name);
	printf("%5ld ", (long)sb.st_size);
	
	//printTime(sb.st_mtime);
	printf(" %s", file);
	putchar('\n');
}
