#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <unistd.h>

void cat (rf)
int rf; /* Reading file descriptor */
{
	int wf; /* writing file descriptor */
	static char *buffer;
	static size_t size; /* to get size of each block */
	struct stat sbuffer; /* to get size from file stats */
	wf = fileno(stdout);

	if (fstat(wf,&sbuffer)){
		err(1,"stdout");
	}
	size = sbuffer.st_blksize;
	/* Allocate space for files being read, and do a err check */
	buffer = malloc(size);

	if (!buffer){
		err(1,0);
	}

	ssize_t readBytes, writtenBytes;/* signed size_t */
	int offset = 0; 	/* if write fails offset will be used to keep the stopping point of file */
	readBytes = read(rf, buffer, size);

	while (readBytes > 0){
		/* continue until the same number of bytes read have been written */
		for (offset = 0; readBytes > 0; readBytes -= writtenBytes, offset += writtenBytes){ 
			writtenBytes = write(wf, buffer+offset, readBytes);
			if (writtenBytes < 0){
				err(1, "stdout"); 
			}
		}
		readBytes = read(rf, buffer, size);
	}
}

int main (int argc, char *argv[]){

	int fd;  /* File descriptor */
	++argv; /* skip the first argument */
	fd = fileno(stdin);
	do{
		if(*argv){
			if (strcmp(*argv,"-") == 0){ /* if two strings match return 0 */
				fd = fileno(stdin); 
			} else{
			fd = open(*argv, O_RDONLY); /* Open file in read only mode */
			}
			if(fd < 0){ 
				err(1,"%s", *argv);
			}
			++argv;
		} 
		cat(fd);
	} 
	while (*argv);
	if (fd != fileno(stdin)){
		close(fd); /* close file */
	}
}
