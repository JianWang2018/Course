/* unix domain socket client side 
 * send filename to server. 
 * read the file after obtaining the file descriptor from server.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define BUFLEN 100

int recv_fd(int sockfd) {
    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;
    char a;

    // align address
    union {
	struct cmsghdr cm;
	char control[CMSG_SPACE(sizeof(int))];
    } control_un;

    struct cmsghdr *cmptr;

    // link to cmsghdr
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);

    // dont worry about socket address for stream
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    // to hold data
    iov[0].iov_base = &a;
    iov[0].iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    // read from socket
    if ((n = recvmsg(sockfd, &msg, 0)) <= 0) {
	return(n);
    }

    // retrieve file descriptor from control header
    if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
	cmptr->cmsg_len == CMSG_LEN(sizeof(int))) {
	if (cmptr->cmsg_level == SOL_SOCKET && 
	    cmptr->cmsg_type == SCM_RIGHTS) {
	    return(*((int *)CMSG_DATA(cmptr)));
	}
    }
    return(-1);
}

int main(int argc, char * argv[]) {
    int sockfd, fd,  n;
    struct sockaddr_un addr;
    char buf[BUFLEN];

    if (argc != 3) {
	fprintf(stderr,"USAGE: %s pathname filename.\n", argv[0]);
	fprintf(stderr, "pathname: unix domain socket pathname\n");
	fprintf(stderr, "filename: name of file to be read\n");
	exit(-1);
    }

    if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strncpy(addr.sun_path, argv[1], sizeof(addr.sun_path)-1);
    addr.sun_path[sizeof(addr.sun_path)-1] = '\0';

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
	perror("connect");
	exit(1);
    }

    write(sockfd, argv[2], strlen(argv[2])+1);
    
    fd = recv_fd(sockfd);
    
    if (fd <= 0) {
	fprintf(stderr, "cannot open file\n");
	exit(1);
    }
    printf("fd = %d\n", fd);

    // print content of the file
    while ((n = read(fd, buf, BUFLEN)) > 0) {
	write(STDOUT_FILENO, buf, n);
    }
    return 0;
}




