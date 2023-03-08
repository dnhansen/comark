# Process API and C

## General process stuff

In Linux, each process gets a **process ID**. To obtain the process ID of the running process, use the `getpid` function (provided by the `unistd.h` header file). This has return type `pid_t` (provided by `sys/types.h`), which [according to POSIX](https://man7.org/linux/man-pages/man7/system_data_types.7.html) is a signed integer type. However, there is no conversion specification for `printf` corresponding to `pid_t`, so we cannot simply print it (at least while conforming to the C standard).

However, before printing the process ID we can cast it to the type `intmax_t` (provided by `stdint.h`) which is a [signed integer type](https://man7.org/linux/man-pages/man7/system_data_types.7.html) capable of representing any signed integer type. This type has length modifier `j`, so since it is a signed integer type, its conversion specification is `%jd`. See [`pid.c`](pid.c) for example usage.

If we are not interested in writing portable code, we can of course simply cast the process ID to a concrete signed integer type that we know can represent the type `pid_t` on the implementation in question. To get the local size of `pid_t`, we can simply run

    printf("pid_t: %zu\n", sizeof(pid_t));

TODO: `fork()`, `wait()`, `exec()`, `open()`, `close()`, maybe more.
