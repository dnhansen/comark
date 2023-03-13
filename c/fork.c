#include <stdio.h>
#include <stdint.h>		// intmax_t
#include <stdlib.h>		// exit
#include <unistd.h>		// getpid

int main() {
	printf("%jd: Parent\n", (intmax_t) getpid());
	printf("FORK\n");
	int rc = fork();

	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child
		printf("%jd: Child\n", (intmax_t) getpid());
	} else {
		// parent
		printf("%jd: Parent\n", (intmax_t) getpid());
	}
	return 0;
}
