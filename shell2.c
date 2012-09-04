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
#include <pthread.h>

int trim(char *str);


// Used time functions from a CS 2303 lab

// timeval struct
struct _timeval {
	time_t tv_sec; // seconds
	suseconds_t tv_usec; // microseconds
};

typedef struct _timeval timeval;

struct job {
	pid_t id;
	char name[32];
};
//typedef struct _job job;

// function prototype
timeval* duration(timeval *t1, timeval *t2);
char* buildPath(char *name);
int execute(int argc, char *argv[], char *envp[]);

int main(int argc, char *argv[], char *envp[]) {
	char inbuf[128];
	char *args[32];
	char *cd;
	pid_t process;
	struct rusage p_usage;
	pid_t lastProcess;
	int background;
	int hang;
	struct job jobs[32];
	int numJobs = 0;
	int i;
	for (i = 0; i < 32; ++i)
	{
		jobs[i].id = 0;
		strcpy(jobs[i].name,"");
	}

	//argv++;
	//execute(argc,argv,envp);
	while(1){
		printf("==>");
		fgets(inbuf, sizeof(inbuf), stdin);
		//printf("in:[%s]\n",inbuf);
		if(strcmp("\n",inbuf) == 0){
			continue;
		} else if(strncmp("exit",inbuf,4) == 0){
			break;
		} else if (strncmp("cd ",inbuf,3) == 0){
			trim(inbuf);
			cd = inbuf+3;
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
		} else if (strncmp("jobs",inbuf,4) == 0){
			for (i = 0; i < 32; ++i)
			{
				if (jobs[i].id >0)
				{
					printf("[%i]: %s\n", jobs[i].id, jobs[i].name);
				}
			}
		}
		background = trim(inbuf);
		splitStr(inbuf,args);
		lastProcess = execute(argc,args,envp);

		if (background){
			hang = WNOHANG;
			jobs[(numJobs % 32)].id = lastProcess;
			strcpy(jobs[(numJobs % 32)].name,args[0]);
			numJobs++;

		} else {
			hang = 0;
		}

		while((process = wait3(NULL,hang,&p_usage)) > 0){
			printf("process %i completed\n",process);
			if (!background && process == lastProcess){
				break;
			}
		}
	}

	return 0;
}

int execute(int argc, char *argv[], char *envp[]){
	struct timeval t1; // time struct 1
	struct timeval t2; // time struct 2
	char strtime1[80]; // timestamp 1
	char strtime2[80]; // timestamp 2
	char* path;

	path = buildPath(argv[0]);
	//printf("path:[%s]\n",path);

	gettimeofday(&t1, NULL); // record first timestamp
	pid_t pid = fork();
	if (pid == 0) { // child
		execve(path, argv, envp);		
	} else if (pid > 0) { // parent
		return pid;
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
    }
    //free(pathList);
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

int trim(char *str) {
	int background = 0;
  	char *end = str + strlen(str) - 1;
  	while ((end > str) && (isspace(*end) || (*end == '&'))) {
  		if((*end) == '&'){
  			background = 1;
  			end--;
  		}
  		end--;
  	}
  	*(end + 1) = '\0';
  	return background;
}
