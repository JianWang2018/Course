/************************************************* 
 * this code has a number of issues for nonblocking I/O.
 * in particular, it assume a write will push all the data out in the
 * buffer sucessfully. We should check the error from write.
 * Read UNP section 16.2 for a complete example.
 ************************************************/
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <algorithm>

using namespace std;

//const unsigned serv_port = 5100;
const unsigned MAXBUFLEN = 512;


int doconnect(const struct addrinfo *res) {
    int sockfd;
    int ioflags, n, error;
    fd_set rset, wset;
    socklen_t len;

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) 
	return(-1);
    ioflags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, ioflags | O_NONBLOCK);

    if ((n = connect(sockfd, res->ai_addr, res->ai_addrlen)) < 0) {
	if (errno != EINPROGRESS) {
	    close(sockfd);
	    return(-1);
	}
    }
    if (n == 0) 
	return(sockfd);

    // cannot establish the connection immediately
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    
    select(sockfd+1, &rset, &wset, NULL, NULL);
    // should check signal interrupt
    error = 0;
    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
	len = sizeof(error);
	if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
	{
	    close(sockfd);
	    return(-1);
	}
    }
    
    if (error) {
	close(sockfd);
	return(-1);
    }
    return(sockfd);
}

int main(int argc, char **argv) {
    int sockfd, rv, flag;
    ssize_t n;
    char buf[MAXBUFLEN];
    struct addrinfo hints, *res, *ressave;
    fd_set readfds, masters;
    int maxfd;

    if (argc != 3) {
	cout << "echo_client server_name_or_ip port" << endl;
	exit(1);
    }

    cout << argv[1] << " " << argv[2] << endl;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
	cout << "getaddrinfo wrong: " << gai_strerror(rv) << endl;
	exit(1);
    }

    flag = 0;
    ressave = res;
    do {
	sockfd = doconnect(res);
	if (sockfd > 0) {
	    flag = 1;
	    break;
	}
    } while ((res = res->ai_next) != NULL);
    freeaddrinfo(ressave);

    if (flag == 0) {
	perror("connect");
	exit(1);
    }

    // set stdin to nonblocking mode
    flag = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK);

    FD_ZERO(&masters);
    FD_SET(STDIN_FILENO, &masters);
    FD_SET(sockfd, &masters);
    maxfd = max(STDIN_FILENO, sockfd);

    string oneline;
    for (;;) {
	readfds = masters;
	select(maxfd+1, &readfds, NULL, NULL, NULL);
	
	if (FD_ISSET(STDIN_FILENO, &readfds)) {
	    // input from user
	    getline(cin, oneline);
	    if (oneline == "quit") {
		close(sockfd);
		break;
	    } else {
		// send to the server.
		if (write(sockfd, oneline.c_str(), oneline.length()) ==
		    -1) {
		    perror("write ");
		    close(sockfd);
		    break;
		}
	    }
	}
	
	if (FD_ISSET(sockfd, &readfds)) {
	    // returned message from the server
	    n = read(sockfd, buf, MAXBUFLEN);
	    if (n <= 0) {
		if (n == 0) 
		    cout << "connection closed" << endl;
		else 
		    perror("something wrong ");
		close(sockfd);
		break;
	    }
	    buf[n] = '\0';
	    cout << buf << endl;
	}
    }
}
