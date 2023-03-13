#include <stdio.h>
#include <stdint.h>		// intmax_t
#include <stdlib.h>		// exit
#include <unistd.h>		// getpid

int main() {
	int x = 0;
	printf("%jd: Parent, x = %d\n", (intmax_t) getpid(), x);
	printf("FORK\n");
	int rc = fork();

	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child
		x = 1;
		printf("%jd: Child, x = %d\n", (intmax_t) getpid(), x);
		sleep(2);
		printf("%jd: Child, x = %d\n", (intmax_t) getpid(), x);
	} else {
		// parent
		sleep(1);
		printf("%jd: Parent, x = %d\n", (intmax_t) getpid(), x);
		x = 2;
		printf("%jd: Parent, x = %d\n", (intmax_t) getpid(), x);
	}
	return 0;
}
