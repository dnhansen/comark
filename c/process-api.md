# Process API and C

## General process stuff

In Linux, each process gets a **process ID**. To obtain the process ID of the running process, use the [`getpid(2)`](https://man7.org/linux/man-pages/man2/getpid.2.html) function (provided by the `unistd.h` header file). This has return type `pid_t` (provided by `sys/types.h`), which [according to POSIX](https://man7.org/linux/man-pages/man7/system_data_types.7.html) is a signed integer type. [TODO find what it is on my system.] However, there is no conversion specification for `printf` corresponding to `pid_t`, so we cannot simply print it (at least while conforming to the C standard).

However, before printing the process ID we can cast it to the type `intmax_t` (provided by `stdint.h`) which is a [signed integer type](https://man7.org/linux/man-pages/man7/system_data_types.7.html) capable of representing any signed integer type. This type has length modifier `j`, so since it is a signed integer type, its conversion specification is `%jd`. See [`pid.c`](pid.c) for example usage.

If we are not interested in writing portable code, we can of course simply cast the process ID to a concrete signed integer type that we know can represent the type `pid_t` on the implementation in question. To get the local size of `pid_t`, we can simply run

    printf("pid_t: %zu\n", sizeof(pid_t));


### Errors

System calls and some library functions set the [`errno(3)`](https://man7.org/linux/man-pages/man3/errno.3.html) variable (provided by `errno.h`). This is an integer variable representing which error has occurred, and it can be converted to a human-readable string using the [`perror(3)`](https://man7.org/linux/man-pages/man3/perror.3.html) (from `stdio.h`) or [`strerror(3)`](https://man7.org/linux/man-pages/man3/strerror.3.html) functions.


TODO: `fork()`, `wait()`, `exec()`, `open()`, `close()`, maybe more.


## File management

### Opening files

Each process has a list of open files, each of which is identified by its **file descriptor**. To open a file we use the [`open(2)`](https://man7.org/linux/man-pages/man2/openat.2.html)    (provided by `fcntl.h`) system call which has the signatures

    int open(const char *pathname, int flags);
    int open(const char *pathname, int flags, mode_t mode);

The `flags` argument is usually specified by or'ing some the provided flags, which are divided into **file creation flags** and **file status flags**. The former include the `O_CREAT` flag which creates a new file if `pathname` does not exist. The latter include the **access modes** `O_RDONLY`, `O_WRONLY` and `O_RDWR`, which request opening the file read-only, write-only, or read/write respectively. One of these access modes must be included in `flags`. Multiple flags can be specified by bitwise-or'ing them.

The return value of `open` is the file descriptor of the file, which will be the lowest-numbered file descriptor not currently open for the process.

The `mode` parameter specifies the file mode of the file. [According to POSIX](https://man7.org/linux/man-pages/man7/system_data_types.7.html), `mode_t` is an integer type. Contrary to e.g. `pid_t`, the standard does not specify whether it is signed or unsigned. Furthermore, since it contrary to the process ID is a *parameter* type and not a *return* type, we do need to know something about our system to use it. In practice this is easy since we know which values make sense for `mode`, but this in general makes it difficult to write portable code.

Luckily, POSIX also specifies the behaviour exhibited by `open` given different values of `mode`. For instance, the value `00700` means that the user has read, write and execute permission. Furthermore, several symbolic constants are provided (see the manual page for details).

If neither `O_CREAT` nor `O_TMPFILE` is specified in `flags`, then we may omit `mode`. Otherwise it must be supplied, since if it is not then some arbitrary bytes from the stack will be applied as the file mode instead.


### Closing files

To close a file, use the [`close(2)`](https://man7.org/linux/man-pages/man2/close.2.html) system call (provided by `unistd.h`) with signature

    int close(int fd);

Here `fd` is the file descriptor of the file to be closed, and the return value is `0` upon success and `-1` upon error. In the later case `errno` is set appropriately.


### File offsets

Any open file has a **file offset**, which specifies from where the process will read and to where it will write. Upon reader or writing the offset is updated such that e.g. subsequent reads will read fresh bytes. The offset can be modified by [`lseek(2)`](https://man7.org/linux/man-pages/man2/lseek.2.html) (provided by `unistd.h`). [TODO lseek]

TODO read, write, pread, pwrite