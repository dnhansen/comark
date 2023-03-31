# Week 9 (calendar week 13)

## General notes

Chapter 7 in Hailperin's book assumes that one has read the previous six chapters, which we of course haven't. We will return to threads and concurrency later, but it seems like the chapter on virtual memory will not be assigned reading. This is a shame, since it makes very good reading! See also the recommendations for further reading [below](#further-reading).

Instead, Peyman covers virtual memory in the lectures. If you wish to follow the lectures instead of reading the book, I recommend watching (at least) the parts of the lecture covering slides 9-12, as well as slide 14 up until 'Issues'. In my own experience, the rest of the material is perhaps best digested by reading Hailperin's book (or the literature mentioned [below](#further-reading)) slowly.


## Exercises

### Task 3

You either know this or you don't. Memory is allocated in blocks.


### Task 10


Even though

-  [`syscall`(2)](https://man7.org/linux/man-pages/man2/syscall.2.html) contains information about *how* to perform system calls on different architectures,
-  [`syscalls`(2)](https://man7.org/linux/man-pages/man2/syscalls.2.html) gives us a list of system calls, and
-  the man page for each system call tells us its arguments and return value,

nowhere in the man pages do we find the numbers of each system calls. These vary between architectures and are found various places. On the Raspberry Pi these seem to be found in the `/usr/include/arm-linux-gnueabihf` folder. Particularly in the file `asm/unistd-common.h` which references a macro defined in `asm/unistd.h`. But precisely how these get included in a C program during preprocessing I haven't been able to figure out.


## Hand-in

### Task 1

When we use virtual memory, to read the contents in memory at some *virtual* memory address we must first translate this address to a *physical* memory address.

This task then asks: If we as programmers perform one memory access (imagine we execute a `str` or `ldr` instruction), how many memory accresses are actually performed in total.


### Task 3

Note that since the TLB is part of the MMU, accessing the TLB does *not* count as a memory access. (Indeed, if it did then this task would have no answer!)


### Task 5

Recall that one unit of time equals one memory access.

In this task I am very satisfied with a small upper bound on the average time per memory access, and it is relatively straightforward to find a very small upper bound: Find an upper bound on how often the TLB is updated (hint: how large is a page?), and find an upper bound on the number of memory accesses for every such update (hint: Task 1).


### Task 6

To find $t$, think back to Task 4.

Of course, $X$ has to be large enough so that when the program 'loops back' and accesses $X[0]$ for the second (or third, fourth, etc.) time, the address of $X[0]$ has been discarded from the buffer. How many times do we need to read from $X$ before looping back to ensure that this is the case? (Hint: Consider how many entries are added to the TLB with each memory access (except the first).)


## Further reading

Hailperin's book is very good, but the order in which we cover the material makes it slightly unoptimal reading. For a very readable introduction to processes and virtual memory, I very much like

1. Arpaci-Dusseau, *Operating Systems: Three Easy Pieces* (also known as 'OSTEP'). The three 'pieces' are virtualisation (including processes and virtual memory), concurrency (here we meet threads) and persistence (where the authors discuss I/O, file systems, those kinds of things). The book is freely available [as a PDF](https://pages.cs.wisc.edu/~remzi/OSTEP/).

Relevant for us this week are chs. 4-6 (and 3 for fun) on processes, and chs. 13-15 (and 12 for fun) on virtual memory. Chs. 16-24 cover much more about virtual memory which is also highly recommended reading, but my feeling is that there is rather more material here than is necessary for the course.
