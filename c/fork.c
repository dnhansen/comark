#include <stdio.h>
#include <sys/types.h> // for pid_t
#include <unistd.h> // for getpid
#include <stdint.h> // for intmax_t

int main(int argc, char const *argv[]) {
    pid_t pid = getpid();
    printf("%jd\n", (intmax_t) pid);
    return 0;
}
