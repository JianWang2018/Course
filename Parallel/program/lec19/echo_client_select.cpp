#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string>
#include <algorithm>

using namespace std;

//const unsigned serv_port = 5100;
const unsigned MAXBUFLEN = 512;


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
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) 
	    continue;
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
	    flag = 1;
	    break;
	}
	close(sockfd);
    } while ((res = res->ai_next) != NULL);
    freeaddrinfo(ressave);

    if (flag == 0) {
	perror("connect");
	exit(1);
    }

    FD_ZERO(&masters);
    FD_SET(fileno(stdin), &masters);
    FD_SET(sockfd, &masters);
    maxfd = max(fileno(stdin), sockfd);

    string oneline;
    for (;;) {
	readfds = masters;
	select(maxfd+1, &readfds, NULL, NULL, NULL);
	
	if (FD_ISSET(fileno(stdin), &readfds)) {
	    // input from user
	    getline(cin, oneline);
	    if (oneline == "quit") {
		close(sockfd);
		break;
	    } else {
		// send to the server. may still block here
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
