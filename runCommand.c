// Dylan Kirby djkirby
// David Mihal dimihal
// runCommand.c

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

// Used time functions from a CS 2303 lab

// timeval struct
struct _timeval {
	time_t tv_sec; // seconds
	suseconds_t tv_usec; // microseconds
};

typedef struct _timeval timeval;

// function prototype
timeval* duration(timeval *t1, timeval *t2);
char* buildPath(char *name);

int main(int argc, char *argv[], char *envp[]) {
	struct timeval t1; // time struct 1
	struct timeval t2; // time struct 2
	char strtime1[80]; // timestamp 1
	char strtime2[80]; // timestamp 2
	char* path;


	char **args = argv;
	args++;

	path = buildPath(argv[1]);
	printf("%s\n",path);

	gettimeofday(&t1, NULL); // record first timestamp
	pid_t pid = fork();
	if (pid == 0) { // child
		execve(path, args, envp);		
	} else if (pid > 0) { // parent
		waitpid(pid, NULL, 0);
		gettimeofday(&t2, NULL); // record second timestamp
		// calculate duration
	  	double secs = ((t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec) / 1000000.0;
	  	long usecs = ((t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec);
	  
	 	/*printf("Timestamp 1: %1d seconds, %1d microseconds\n", t1.tv_sec, t1.tv_usec);
	  	printf("Timestamp 2: %1d seconds, %1d microseconds\n", t2.tv_sec, t2.tv_usec);*/
	  	printf("Elapsed Time: %ld microseconds (~%.3f seconds)\n", usecs, secs);
	  	
	  	struct rusage usage;
	  	struct rusage *ptr = &usage;
	  	getrusage(RUSAGE_CHILDREN, ptr);
	  	printf("Page Faults: %lu\n", ptr->ru_majflt);
	  	
	  	struct timeval time_in_user = ptr->ru_utime;
	  	struct timeval time_in_system = ptr->ru_stime;
	} else { // failed to fork
		return -1;
	}
	return 0;
}

int pathExists(char *cmd) {
	if (access(cmd, F_OK) != -1) { 
		return 1;
	} else {
		return -1;
	}
}

char* buildPath(char *name) {
	if (*name != '~' && *name != '/' && pathExists(name) == -1) {
 		char *path = strtok(getenv("PATH"), ":");

  		while (path != NULL) {
    		char *fullPath = malloc(strlen(path) + strlen(name) + 1);
    		strcpy(fullPath, path);
    		strcat(fullPath, "/");
    		strcat(fullPath, name);
    		//printf("trying: %s\n",fullPath);
    		if (pathExists(fullPath) != -1) {
    			return fullPath;
    		} 
    		path = strtok(NULL, ":");
    	}
    }
    return name;
}

/**
  * Calculates elapsed time between two timestamps
  * @param *t1 First timestamp
  * @param *t2 Second timestamp (must be greater than *t1)
  * @return Pointer to timeval comtaining elapsed time
  */
timeval* duration(timeval *t1, timeval *t2) {
	timeval *t;
	t = malloc(sizeof(timeval));
	t1->tv_sec = (t2->tv_sec - t1->tv_sec);
	t1->tv_usec = (t2->tv_usec - t1->tv_usec);
	return t;
}
