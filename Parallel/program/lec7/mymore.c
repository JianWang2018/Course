#include <stdio.h>
#include<sys/types.h>
#include <unistd.h>
#include <errno.h>

main()
{

	int i;

	if( tcgetpgrp(STDIN_FILENO) < 0) {
		printf("errno = %d", errno);
		perror("tcgetpgrp");
	}

	printf("pgrp=%d pid=%d term=%d\n", getpgrp(), getpid(), tcgetpgrp(STDIN_FILENO));

	for( i=0; i<10; i++) {
		printf("line %d\n", i);
		getchar();
	}
}

