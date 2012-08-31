// Dylan Kirby
// runCommand.c

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[]) {
	pid_t pid = fork();
	if (pid == 0) { // child
	   char **args;
	   args= argv;
	   args++;
	   printf("%s\n",argv[1]);
	   buildPath(argv[1]);
	   execve(argv[1], args, 0);
	} else if (pid > 0) { // parent
		waitpid(pid, NULL, 0);
	} else { // failed to fork
		printf("error");
		return -1;
	}
	return 0;
}
int buildPath(char *cmd[]) {
	if( access( *cmd, F_OK ) != -1 ) {
		printf("good\n");
	} else {
		printf("bad\n");
	}
}
