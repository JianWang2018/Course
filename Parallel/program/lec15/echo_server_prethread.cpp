#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

//const unsigned port = 5100;
const unsigned MAXBUFLEN = 512;
pthread_mutex_t accept_lock = PTHREAD_MUTEX_INITIALIZER;
int serv_sockfd;

void *one_thread(void *arg) {
    int cli_sockfd;
    struct sockaddr_in cli_addr;
    socklen_t sock_len;
    ssize_t n;
    char buf[MAXBUFLEN];

    int tid = *((int *)arg);
    free(arg);

    cout << "thread " << tid << " created" << endl;
    for (; ;) {
	sock_len = sizeof(cli_addr);
	pthread_mutex_lock(&accept_lock);
	cli_sockfd = accept(serv_sockfd, (struct sockaddr *)&cli_addr, &sock_len);
	pthread_mutex_unlock(&accept_lock);

	cout << "thread " << tid << ": ";
	cout << "remote client IP == " << inet_ntoa(cli_addr.sin_addr);
	cout << ", port == " << ntohs(cli_addr.sin_port) << endl;

	while ((n = read(cli_sockfd, buf, MAXBUFLEN)) > 0) {
	    buf[n] = '\0';
	    cout << buf << endl;
	    write(cli_sockfd, buf, strlen(buf));
	}
	if (n == 0) {
	    cout << "client closed" << endl;
	} else {
	    cout << "something wrong" << endl;
	}
	close(cli_sockfd);
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;
    int port, number_thread;
    pthread_t tid;
    int *tid_ptr;

    if (argc != 3) {
	fprintf(stderr, "%s port number_thread\n", argv[0]);
	exit(1);
    }

    port = atoi(argv[1]);
    number_thread = atoi(argv[2]);

    cout << "port = " << port << " number of threads == " <<
	number_thread << endl;
    serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero((void*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    bind(serv_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    listen(serv_sockfd, 5);

    int i;
    for (i = 0; i < number_thread; ++i) {
	tid_ptr = (int *)malloc(sizeof(int));
	*tid_ptr = i;
	pthread_create(&tid, NULL, &one_thread, (void *)tid_ptr);
    }

    for (;;) 
	pause();
}
