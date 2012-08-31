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
	   printf("%s,%s",argv[1],args);
	   execve(argv[1], args, 0);
	} else if (pid > 0) { // parent
		waitpid(pid, NULL, 0);
	} else { // failed to fork
		printf("error");
		return -1;
	}
	return 0;
}
