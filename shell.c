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

char *trim(char *str);


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
int execute(int argc, char *argv[], char *envp[]);

int main(int argc, char *argv[], char *envp[]) {
	char inbuf[128];
	char *args[32];
	char *cd;

	//argv++;
	//execute(argc,argv,envp);
	while(1) {
		printf("==>");
		
		if (fgets(inbuf, sizeof(inbuf), stdin) == NULL)
			break; // seg faults
			
		if (strncmp("exit",inbuf,4) == 0)
			break;
		
		if (strncmp("cd ",inbuf,3) == 0){
			cd = trim(inbuf+3);
			if (*cd=='~'){
				cd++;
				if(*cd=='/'){
					cd++;
				}
				char* tmp = malloc(sizeof(cd));
				strcpy(tmp,cd);
				strcpy(cd,getenv("HOME"));
				strcat(cd,"/");
				strcat(cd,tmp);
				free(tmp);
			}
			if (chdir(cd) == 0){
				printf("Changed to %s\n",cd);
			} else {
				printf("couldn't change to %s\n",cd);
			}
			continue;
		}
		splitStr(trim(inbuf),args);
		execute(argc,args,envp);
	}

	return 0;
}

int execute(int argc, char *argv[], char *envp[]){
	struct timeval t1; // time struct 1
	struct timeval t2; // time struct 2
	char strtime1[80]; // timestamp 1
	char strtime2[80]; // timestamp 2
	char* path;

	if ((path = buildPath(argv[0])) == NULL)
		printf("Illegal command: %s\n", argv[0]);
	
	path = buildPath(argv[0]);
	//printf("path:[%s]\n",path);

	gettimeofday(&t1, NULL); // record first timestamp
	pid_t pid = fork();
	if (pid == 0) { // child
		execve(path, argv, envp);		
	} else if (pid > 0) { // parent
		waitpid(pid, NULL, 0);
		free(path);

		gettimeofday(&t2, NULL); // record second timestamp
		// calculate duration
	  	double secs = ((t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec) / 1000000.0;
	  	long usecs = ((t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec);

	  	printf("Elapsed Time: %ld microseconds (~%.3f seconds)\n", usecs, secs);
	  	
	  	struct rusage usage;
	  	struct rusage *ptr = &usage;
	  	getrusage(RUSAGE_CHILDREN, ptr);
	  	
	  	struct timeval cpu_user_time = ptr->ru_utime;
	  	struct timeval cpu_system_time = ptr->ru_stime;
	  	
	  	
	  	printf("Elapsed user CPU time: %ld microseconds\n", (long)cpu_user_time.tv_usec);
	  	printf("Elapsed system CPU time: %ld microseconds\n", (long)cpu_system_time.tv_usec);
	  	
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
	char pathList[128];
	strcpy(pathList,getenv("PATH"));
	//printf("path:%s\n", pathList);
	if (*name != '~' && *name != '/' && pathExists(name) == -1) {
 		char *path = strtok(pathList, ":");
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
    	return NULL;
    }
    return name;
}
int splitStr(char* needle,char *arr[]){
	int i = 0;
	char* current = strtok(needle," ");
	while (current != NULL) {
		arr[i]= current;
		current = strtok(NULL," ");
		i++;
	}
	arr[i] = NULL;
	return i;
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

char* trim(char *str) {
  	char *end = str + strlen(str) - 1;
  	while (end-- > str && isspace(*end));
  	*(end + 1) = '\0';
  	return str;
}
