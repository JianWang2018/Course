#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

static char str1[] = "OP1\n";
static char str2[] = "AB2\n";
static const int MAXBUF = 1024;

int main() {
	int sockfd[2], pid;
	char buf[MAXBUF];

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
		perror("socketpair");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	if (pid == 0) {
		// child
		close(sockfd[0]);
		if (read(sockfd[1], buf, MAXBUF) < 0) { 
		       perror("read");
	       	       exit(1);
		}
 		printf("Received: %s", buf);

		if (write(sockfd[1], str1, strlen(str1)+1) < 0) {
			perror("write");
			exit(1);
		}
		close(sockfd[1]);		
	} else {
		// parent process 
		close(sockfd[1]);
		if (write(sockfd[0], str2, strlen(str2)+1) < 0) { 
		       perror("write");
	       	       exit(1);
		}

		if (read(sockfd[0], buf, MAXBUF) < 0) {
			perror("read");
			exit(1);
		}
 		printf("Received: %s", buf);
		close(sockfd[0]);		
	}
}
