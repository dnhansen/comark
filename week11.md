# Week 11 (calendar week 16)

## General notes

TODO

## Exercises

See the [manual pages](https://man7.org/linux/man-pages/man1/scp.1.html) for more information on the `scp` command. (Note that if you are on Windows, you have probably installed OpenSSH, so the manual page is also valid on Windows.)


### Task 3

TODO: Something about futex and the relationship to locks in general.


### Task 8

If using condition variables, it may seem tempting to only check once (using an `if` statement) whether all threads have entered the barrier, and then call `pthread_cond_wait` if not. This is probably work in the majority of cases. However, it is possible for `pthread_cond_wait` to return without explicitly calling `pthread_cond_signal` or `pthread_cond_broadcast` (a so-called **spurious wakeup**). Hence it is necessary to recheck the condition (using a `while` loop) every time the thread is awoken.


## Hand-in

TODO

## Further reading

I'm not particularly fond of Hailperin's exposition of concurrency. I personally prefer

1. Arpaci-Dusseau, *Operating Systems: Three Easy Pieces*.

It goes in much greater detail about the types of atomic instructions can be used to implement spin locks. It contains rather less material on deadlocks, however.
