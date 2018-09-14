#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <deque>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

const int MAXDG = 1024;

typedef struct {
    char dg_data[MAXDG];
    size_t dg_len;
    struct sockaddr_in dg_sa;
    socklen_t dg_salen;
} DG;

deque<DG> dg_deque;
int sockfd;

void sig_io(int signo) {
    ssize_t len;
    
    // signal SIGIO is blocked while in the handler
    while (1) {
	DG dg_tmp;

	dg_tmp.dg_salen = sizeof(dg_tmp.dg_sa);
	len = recvfrom(sockfd, dg_tmp.dg_data, MAXDG, 0,
		       (struct sockaddr *)&(dg_tmp.dg_sa), &(dg_tmp.dg_salen));
	
	if (len < 0) {
	    if (errno == EWOULDBLOCK)
		break; // done with all pending datagrams
	    else {
		perror("recvfrom");
		return;
	    }
	}
	dg_tmp.dg_len = len;

	dg_deque.push_back(dg_tmp);
    }
}

int main(int argc, char *argv[]) {

    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    const int on = 1;
    sigset_t zeromask, newmask, oldmask;


    if (argc != 2) {
	fprintf(stderr, "%s port\n", argv[0]);
	exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons((int)atoi(argv[1]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("socket");
	exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) <
	0) {
	perror("bind");
	exit(1);
    }

    signal(SIGIO, sig_io); // step one: set signal handler
    fcntl(sockfd, F_SETOWN, getpid()); // step two: set socket owner 
    ioctl(sockfd, FIOASYNC, &on); // step three: signal-driven I/O
    ioctl(sockfd, FIONBIO, &on); // nonblocking I/O

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigemptyset(&oldmask);
    sigaddset(&newmask, SIGIO);

    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    while (1) {
	while (dg_deque.empty()) {
	    // waiting for signals
	    // unblock signals when it is called. It also saved the old
	    // signal mask, and set it back when the function returns.
	    sigsuspend(&zeromask);
	}

	// we do not need to block SIGIO when we send datagram (in
	// position 0
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	sendto(sockfd, dg_deque[0].dg_data, dg_deque[0].dg_len, 0,
	       (struct sockaddr *)&(dg_deque[0].dg_sa), dg_deque[0].dg_salen);
	
	// block signal SIGIO to delete element from dg_deque
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	dg_deque.pop_front();
    }
}
