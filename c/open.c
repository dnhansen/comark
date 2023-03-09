#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close

int main(void)
{
    int fd = open("testfile.txt", O_CREAT|O_RDWR, S_IRWXU);
    char *hello = "Hello world\n";
    size_t n = 12;
    ssize_t written = write(fd, (const void *)hello, n);
    printf("Bytes written: %zd\n", written);
    close(fd);

    int fd2 = open("testfile.txt", O_CREAT|O_RDWR, S_IRWXU);
    
    char readBytes[12];
    
    read(fd2, (void *)readBytes, written);
    printf("%s", readBytes);

    close(fd2);
    
    return 0;
}
