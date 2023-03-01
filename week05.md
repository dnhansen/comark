# Week 5 (calendar week 9)

## General notes

### System calls

We will see system calls again later when we study operating systems in more detail. A **system call** is a function that asks the operating system for a particular service, like writing to or reading from a file, exiting a program, creating or removing directories, updating access permissions, and so on.

On the Linux manual page for the [`syscall`](https://man7.org/linux/man-pages/man2/syscall.2.html) function we can see the **calling conventions** for different kinds of architectures. The Raspberry Pi uses arm/EABI (to see this, consider the output of the `readelf -h max` command from Task 4 of this week's exercises), so to perform a system call we use the instruction `swi 0x0` (`swi` for 'SoftWare Interrupt'). Note that `swi` is equivalent to the instruction `svc` (for 'SuperVisor Call') used by the Unified Assembler Language (UAL). The table also indicates which register should be used to specify the system call (which on arm/EABI is `r7`), and which registers receive return values or error values.

Looking at [a list of system calls](https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/syscalls.md#arm-32_bit_eabi), we see that the code for the `write` system call is `0x04`, and that the code for the `exit` system call is `0x01`. The table also specifies in which registers arguments to the system call should be placed. For instance, the arguments to `write` should be placed in registers `r0`, `r1` and `r2`.

Furthermore, looking at the manual page for the [`write`](https://man7.org/linux/man-pages/man2/write.2.html) system call, we see what each argument does: The first argument `fd` is a **file descriptor**. On Linux many things are modelled using files, for instance standard I/O. Hence the standard output also gets a file descriptor, and it is always given the descriptor `1`. The argument `buf` is a pointer to the string to be printed, and `count` is the size of the string.

Similarly, the `exit` system call takes exactly one argument to be placed in `r0`, namely the exit code (which is usually `0` when no error has occurred, and something else when an error has in fact occurred, the code depending on the error).

You can compare these with the system calls performed in the program `max.s` in this week's exercises.


### Branching

Simple branching is done with the `b` instruction. The syntax is

    b{cond} label

where `{cond}` is an optional **condition code**. If `{cond}` is omitted, the branch is unconditional.

Examples of condition codes include `eq` and `ne`, which branch to `label` if the `Z` flag is set or clear, respectively. See [the documentation](https://developer.arm.com/documentation/dui0068/b/ARM-Instruction-Reference/Conditional-execution) for a complete list of condition codes.

We need to specify whether an instruction should set the flags or not. Most instructions, e.g. `add` and `sub`, do not set the flag, but adding the suffix `s` makes them set the flag.

For instance, the instructions

    subs r1, r1, 1
    beq zero

thus subtract 1 from `r1` and branches to the label `zero` if the result is zero.

The instruction [`cmp`](https://developer.arm.com/documentation/dui0068/b/ARM-Instruction-Reference/ARM-general-data-processing-instructions/CMP-and-CMN) may also be useful for comparing values.


## Exercises

### Task 9

We place the sum in the register `r0`, since this is the register from which the `exit` system call get its input, the exit code.


### Task 10

There is a typo in the example: The first dollar sign should not be there! To be clear, we use the command `echo $?` to print the exit code of the previous command. Note that it does *not* work to write something like

    ./array && echo $?

Evaluation proceeds left to right and `&&` short-circuits, which means that evaluation ends when a process evaluates to something 'falsy' (since `&&` is supposed to be an AND, the entire expression is false if just one of the subexpressions is false). But a command is 'truthy' if its exit code is `0`, and 'falsy' otherwise. Since we use the exit code as output for our program, we obviously can't rely on it being `0`!

In this week's hand-in we will learn how to properly print numbers!


## Hand-in

As mentioned above, the `write` system call takes as arguments a file descriptor (in `r0`, here standard output), the addres of the string to be printed (in `r1`), and the size in bytes of the string to be printed (in `r2`). This explains the values we place into the registers in the `single-dig.s` program.

Furthermore, by 'exit gracefully' we just mean that the program exits with exit code `0`, indicating that no error has occurred. Compare the `array.s` program from the exercises where the exit code would appear to indicate that an error actually had occurred!

To understand how to use the `strb` instruction, recall from the exercises how multi-byte numbers are represented on the Raspberry Pi. Why can we store the eight least significant bits of a 32-bit number?
