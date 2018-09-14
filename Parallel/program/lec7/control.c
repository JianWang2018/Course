#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>


int status, child_pid;

void int_handler(int arg) 
{
	printf("Child terminated.---..\n");
}

main()
{

	printf("pgrp=%d pid=%d term=%d\n", getpgrp(), getpid(), tcgetpgrp(STDIN_FILENO));

           /* Ignore interactive and job-control signals.  */
       
	   signal (SIGQUIT, SIG_IGN);
           signal (SIGTSTP, SIG_IGN);
           signal (SIGTTIN, SIG_IGN);
           signal (SIGTTOU, SIG_IGN);
           signal (SIGCHLD, SIG_IGN);
	   //           signal (SIGINT, SIG_IGN);
	   

#if 0
	if( signal(SIGTSTP, tstp_handler) == SIG_ERR) {
		perror("signal 1");
		exit(1);
	}

	if( signal(SIGCHLD, chld_handler) == SIG_ERR) {
		perror("signal 2");
		exit(1);
	}

	if( signal(SIGINT, int_handler) == SIG_ERR) {
		perror("signal 2");
		exit(1);
	}
#endif

	/* Make your shell its process group leader */
	if (setpgid (getpid(), getpid()) < 0) {
            perror ("setpgid 1");
            exit (1);
	}

	/* Get terminal control from the  shell that started your shell */
	if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
		perror("tcsetpgrp 1");
		exit(1);
	}

	if( (child_pid = fork()) < 0) {
		perror("fork:");
		exit(1);
	}

	if( child_pid == 0) { /* child */
           signal (SIGQUIT, SIG_DFL);
           signal (SIGTSTP, SIG_DFL);
           signal (SIGTTIN, SIG_DFL);
           signal (SIGTTOU, SIG_DFL);
           signal (SIGCHLD, SIG_DFL);
           signal (SIGINT, SIG_DFL);

		/* Make yourself process group leader */
		if( setpgid(0,0) < 0) {
			perror("setpgid 2");
			exit(1);
		}

		/* transfer controlling terminal */
		if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
			perror("child tcsetpgrp");
			exit(1);
		}

		if (execlp("./mymore", "./mymore", "/tmp/x", (char *)0) < 0) {
			perror("execlp");
			exit(1);
		}
		exit(2);

	} else /* parent */ {

		/* Make child its own process group leader  - to avoid race conditions */
		if( setpgid(child_pid, child_pid) < 0) {
			if( errno != EACCES ) {
				perror("setpgid 3");
				exit(1);
			}
		}

           signal (SIGTTIN, int_handler);

	   // do 
		 {
			/* transfer controlling terminal */
			if ( tcsetpgrp (STDIN_FILENO, getpgid(child_pid)) < 0) { /* we also do this in child */
				perror("tcsetpgrp 2");
				exit(1);
			}

			if ( kill(child_pid, SIGCONT) < 0) {
				perror("kill");
			}


                        wait(&status);

			/* Children completed: put the shell back in the foreground.  */
			if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
				perror("tcsetpgrp 2");
				exit(1);
			}
			

			printf("Parent\n");

			if( WIFSTOPPED(status) )
				printf("Child process stopped\n");
			else {
				printf("Child process terminated. Now in parent. Press any key to terminate.\n");
				getchar();
			}

			//printf("Type something: \n");
			//printf("Parent got %c\n", getchar());

		 } //while(WIFSTOPPED(status));

	}
}

