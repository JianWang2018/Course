#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <list>

using namespace std;

//const unsigned port = 5100;
const unsigned MAXBUFLEN = 512;


int main() {
    int serv_sockfd, cli_sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t sock_len;
    pid_t child_pid;
    ssize_t n;
    char buf[MAXBUFLEN];
    fd_set readfds, masters;
    int maxfd;
    list<int> sockfds;
    
    // cout << "port = " << port << endl;
    serv_sockfd = socket(PF_INET, SOCK_STREAM, 0);
    
    bzero((void*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(0);

    bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    listen(serv_sockfd, 5);
    
    sock_len = sizeof(cli_addr);
    getsockname(serv_sockfd, (struct sockaddr *)&serv_addr, &sock_len);
    cout << "port = " << ntohs(serv_addr.sin_port) << endl;

    FD_ZERO(&masters);
    FD_SET(serv_sockfd, &masters);
    maxfd = serv_sockfd;
    sockfds.push_back(serv_sockfd);

    for (; ;) {
	readfds = masters;
	select(maxfd+1, &readfds, NULL, NULL, NULL);
	
	list<int> l_tmp(sockfds);
	list<int>::iterator itr;
	for (list<int>::iterator itr = l_tmp.begin(); itr !=
		 l_tmp.end(); ++itr) {
	    int sock_tmp = *itr;

	    if (FD_ISSET(sock_tmp, &readfds)) {
		if (sock_tmp == serv_sockfd) {
		    // new connection request
		    cli_sockfd = accept(serv_sockfd, (struct sockaddr
						      *)&cli_addr, &sock_len);
		    cout << "New connection accepted" << endl;
		    FD_SET(cli_sockfd, &masters);
		    if (cli_sockfd > maxfd)
			maxfd = cli_sockfd;
		    sockfds.push_back(cli_sockfd);
		} else {
		    // data message
		    n = read(sock_tmp, buf, MAXBUFLEN);
		    if (n <= 0) {
			if (n == 0) 
			    cout << "connection closed" << endl;
			else
			    perror("something wrong");
			close(sock_tmp);
			FD_CLR(sock_tmp, &masters);
			sockfds.remove(sock_tmp);
		    } else {
			buf[n] = '\0';
			cout << buf << endl;
			write(sock_tmp, buf, strlen(buf));
		    }
		}
	    }
	}
    }
    close(serv_sockfd);
}

