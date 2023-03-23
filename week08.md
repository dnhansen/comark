# Week 8 (calendar week 12)

## General notes

In this week's exercises we have to supply powers of two as command line arguments to an executable. Instead of computing the value of such a power and pasting this into the terminal, we can instead perform arithmetic directly in the terminal.


### Arithmetic in Bash on Linux (and Mac)

The default shell on most Linux distributions (including Ubuntu, as well as the distribution running on our Raspberry Pi) is Bash (see [`bash`(1)](https://www.man7.org/linux/man-pages/man1/bash.1.html)). If `<expression>` is some arithmetic expression, then

    $((<expression>))

evaluates the expression. You can use this to compute powers of two and pass the resulting value as an argument to a command. For instance, the command

    ./myprogram $((2**8))

will pass the power $2^8$ to the program `myprogram`. Note that we use `**` and not `^` to denote exponentiation.


### Arithmetic on Mac

The default shell on macOS is Zsh which is similar to Bash, but I haven't used it myself. The above may also work here.


### Arithmetic in PowerShell on Windows

In PowerShell we don't need to use the `$`-syntax, but as far as I know PowerShell does not have an exponentiation operator. Instead we can use left shifts:

    ./myprogram.exe (1 -shl 8)


## Hand-in

### Task 1

Note that there was a mistake in the first version of `Bst.java`, so make sure you have the latest version.


### Task 2

Just as you don't have to know binary search before doing this task, you also don't need to explain binary search in your solution!

The method `compBST` should be easy enough to understand, especially if you already know about binary search.


### Task 3

The method `slcompBST` (`sl` for 'slanted') is similar to `compBST`, but notice the role played by the `slFactor` parameter.

To understand `predBST`, first recall that `int` is a 32-bit signed integer type. Since the operator `>>` denotes an *arithmetic* right shift (see [my notes for week 4](week04.md) for the distinction between arithmetic and logical right shifts), what does the value of `(x-A[mid])>>31` tell us about `x-A[mid]`? And what does that tell us about how `x` and `A[mid]` relate to each other? Finally recall that `&` is bitwise AND.

When comparing expected runtimes, it may be useful to think back to the exercises in week 4. Of course don't immediately disregard your naïve expectations either, in particular in the computations of `left` and `right` in each of the methods.

Also see the comment below the description of Task 4.


### Task 4

When running the programs from the terminal, the syntax for evaluating arithmetic expressions mentioned [above](#general-notes) may be helpful.
