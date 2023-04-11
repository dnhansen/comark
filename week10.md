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


### Task 8

On my machine, redirecting standard out somehow represses (some of) the `write` system calls, at least in the standard error output of `strace`. If you experience something similar, try omitting the redirect.


### Task 12

It is sufficient to execute the command `man brk` since there is no `man 1 brk`. It is perhaps useful to also take a look at the 'NOTES' section, in particular the subsection 'C library/kernel differences'.

Apparently the system call `brk` returns the location of the (current or new) break point, while there is a wrapper which returns the status code as described. Since `strace` prints the return value of the system call itself, we can use it to see the break point.


## Hand-in

TODO

## Further reading

TODO