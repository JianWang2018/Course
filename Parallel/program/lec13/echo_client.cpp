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

using namespace std;

//const unsigned serv_port = 5100;
const unsigned MAXBUFLEN = 512;


int main(int argc, char **argv) {
    int sockfd, rv, flag;
    ssize_t n;
    char buf[MAXBUFLEN];
    struct addrinfo hints, *res, *ressave;

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

    ressave = res;
    flag = 0;
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
	fprintf(stderr, "cannot connect\n");
	exit(1);
    }
    string oneline;
    while (getline(cin, oneline)) {
	if (oneline == "quit") {
	    close(sockfd);
	    break;
	} else {
	    write(sockfd, oneline.c_str(), oneline.length());
	    n = read(sockfd, buf, MAXBUFLEN);
	    buf[n] = '\0';
	    cout << buf << endl;
	    
	    if (n <= 0) {
		if (n == 0) {
		    cout << "server closed" << endl;
		} else {
		    cout << "something wrong" << endl;
		}
		close(sockfd);
		exit(0);
	    }
	}
    }
}

