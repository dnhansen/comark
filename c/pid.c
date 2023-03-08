#include <stdio.h>
#include <sys/types.h>  // pid_t
#include <unistd.h>     // getpid
#include <stdint.h>     // intmax_t

int main(void) {
    pid_t pid = getpid();
    printf("%jd\n", (intmax_t) pid);
    return 0;
}
