# Week 10 (calendar week 15)

## General notes

This week we will be executing a bunch of commands in the terminal, many of them similar with similar arguments. In Bash (the default terminal on the Raspberry Pi) there are various tools to help with productivity. The default macOS shell is Zsh which is similar to Bash, and the tricks below probably work in Zsh as well.


### Tab completion

Remember that pressing the tab key performs autocompletion if there is only one match. If there are multiple matches, then pressing tab twice shows all the matches.


### Searching command history

Pressing `CTRL+R` allows you to search for commands you have already executed. Press `CTRL+R` repeatedly to browse through the matches.


## Exercises

### Task 1

Note that running the `ps` command without the `-l` flag will also show the `TIME` column.


### Task 6

The `-pthread` flag to `gcc` is necessary on the Raspberry Pi, and it is apparently supposed to be necessary on all platforms. (On my own Linux VM, however, I am able to compile C-programs using the POSIX Threads API without this flag. Who knows why!)

Note that we cast to `intptr_t` and so on for simplicity. It is often done in practice, but the `intptr_t` type (and its corresponding untyped version `uintptr_t`) are not required by the C standard. If you need to write portable code, it is recommended to allocate (either dynamically or on the stack, if this is possible) a piece of memory and pass a pointer to the child.

Note also that we have to be careful when passing a pointer to `child_function`, since if we pass the same pointer to multiple children, then the pointer, along with the value stored at the pointer, will be shared by all children.


### Task 8

On my machine, redirecting standard out somehow represses (some of) the `write` system calls, at least in the standard error output of `strace`. If you experience something similar, try omitting the redirect.


### Task 9

The [`mmap2`(2)](https://man7.org/linux/man-pages/man2/mmap2.2.html) system call is provided by the Linux kernel and supersedes the system call `mmap`. The GNU C Library (glibc) provides a wrapper function [`mmap`(2)](https://man7.org/linux/man-pages/man2/mmap.2.html) which is usually called instead when available (which it obviously isn't on non-GNU systems!).


### Task 10

The [source code](https://elixir.bootlin.com/glibc/glibc-2.28/source/malloc/malloc.c#L1058) for the [`malloc`(3)](https://man7.org/linux/man-pages/man3/malloc.3.html) function in the GNU C Library describes the layout of free and allocated memory chunks. (The command `ldd --version` prints the version of glibc we are using, which is version 2.28. That is, Peyman links to an older version which has slightly different behaviour.)

Essentially, `malloc` allocates an extra two words immediately before the block of memory available to the user. The second word contains the size of the chunk in bytes. From the [source code](https://elixir.bootlin.com/glibc/glibc-2.28/source/malloc/malloc.c#L1174) it is also fairly easy to convince oneself [TODO: Write something about struct alignment and offsets?] that the minimum chunk size is at least 16, so the three least significant bits of the chunk size are zero. Hence we may use these bits to store bit flags.

When you access `A[-1]` you will notice that its value is (always?) 2 larger than the actual size of the allocated chunk, since the second bit is set. This indicates that the chunk was allocated using `mmap`, which indeed it was.

The first word contains the size of the previous chunk, which apparently turns out to be 0 (for reasons I don't understand).

NOTE: Using array indices to access values outside of an array yields undefined behaviour [TODO: or implementation-defined behaviour?], so don't actually do this.


### Task 12

It is sufficient to execute the command `man brk` since there is no `man 1 brk`. It is perhaps useful to also take a look at the ['NOTES' section](https://man7.org/linux/man-pages/man2/brk.2.html#NOTES) of the manual page, in particular the subsection 'C library/kernel differences'.

As with `mmap` there is both a system call `brk` and a wrapper function of the same name provided by the GNU C Library. The *system call* returns the location of the (current or new) break point, the *wrapper* returns the status code as described. Since `strace` prints the return value of the system call itself, we can use it to see the break point.


## Hand-in

The provided source code defines its own Boolean type. In fact the C standard library provides one in the `<stdbool.h>` header file, which we might as well have included instead.

See also [my notes](c-notes.md#the-posix-thread-api) on the POSIX Thread API for information on threads.

A hint recommends defining a struct such as

    typedef struct work_st { /* ... */ } work_t; 

The suffix `_st` is often used for structs, and the suffix `_t` for various types. However, while the former is harmless, one has to be careful when declaring types ending in `_t`. First of all, from the [C standard](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf):

> Typedef names beginning with `int` or `uint` and ending with `_t` may be added to the types defined in the `<stdint.h>` header. (C11, 7.31.10)

In other words, one should avoid declaring types beginning with `int` or `uint` and ending with `_t`, since these may be added to the C standard in the future. This is obviously not an issue here, but the [POSIX standard](https://pubs.opengroup.org/onlinepubs/9699919799/xrat/V4_xsh_chap02.html#tag_22_02_12) says the following:

> To allow implementors to provide their own types, all conforming applications are required to avoid symbols ending in "_t", which permits the implementor to provide additional types.

That is, if one aims for POSIX compliance, then one should avoid declaring types ending in `_t`.


## Further reading

TODO