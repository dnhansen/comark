# Week 5 (calendar week 9)

## General notes

### System calls

We will see system calls again later when we study operating systems in more detail. A **system call** is a function that asks the operating system for a particular service, like writing to or reading from a file, exiting a program, creating or removing directories, updating access permissions, and so on.

On the Linux manual page for the [`syscall`](https://man7.org/linux/man-pages/man2/syscall.2.html) function we can see the **calling conventions** for different kinds of architectures. The Raspberry Pi uses arm/EABI (to see this, consider the output of the `readelf -h max` command from Task 4 of this week's exercises), so to perform a system call we use the instruction `swi 0x0` (`swi` for 'SoftWare Interrupt'). Note that `swi` is equivalent to the instruction `svc` (for 'SuperVisor Call') used by the Unified Assembler Language (UAL). The table also indicates which register should be used to specify the system call (which on arm/EABI is `r7`), and which registers receive return values or error values.

Looking at [a list of system calls](https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/syscalls.md#arm-32_bit_eabi), we see that the code for the `write` system call is `0x04`, and that the code for the `exit` system call is `0x01`. The table also specifies in which registers arguments to the system call should be placed. For instance, the arguments to `write` should be placed in registers `r0`, `r1` and `r2`.

Furthermore, looking at the manual page for the [`write`](https://man7.org/linux/man-pages/man2/write.2.html) system call, we see what each argument does: The first argument `fd` is a **file descriptor**. On Linux many things are modelled using files, for instance standard I/O. Hence the standard output also gets a file descriptor, and it is always given the descriptor `1`. The argument `buf` is a pointer to the string to be printed, and `count` is the size of the string.

Similarly, the `exit` system call takes exactly one argument to be placed in `r0`, namely the exit code (which is usually `0` when no error has occurred, and something else when an error has in fact occurred, the code depending on the error).

You can compare these with the system calls performed in the program `max.s` in this week's exercises.


## Exercises

### Task 9

We place the sum in the register `r0`, since this is the register from which the `exit` system call get its input, the exit code.


### Task 10

There is a typo in the example: The first dollar sign should not be there! To be clear, we use the command `echo $?` to print the exit code of the previous command.

It is apparently not easy to print numbers using assembly. Answers to [this question](https://stackoverflow.com/questions/17357467/how-to-print-a-number-in-arm-assembly) on StackOverflow indicate that using the `printf` function from the C library is the way to go. Hence we use exit codes to get an output instead.


## Hand-in

TODO

## Further reading

TODO