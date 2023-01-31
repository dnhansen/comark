# Week 1 (calendar week 5)

## General notes

The course has had a major overhaul since last year, particularly in the first section on computer architecture. Last year we used the book *Structured Computer Organization* by Tanenbaum, and my feeling is that the new book is a definite upgrade.

### Linux terminal basics

The exercises cover basic Bash commands.

When using the VM in text mode, you only have access to a single terminal. It is not quite obvious how to switch between programs, e.g. if you wish to briefly jump out of an editor to do some file management.

When inside a program (e.g. an editor, an REPL, etc.) press `CTRL+Z` to suspend the program (or 'job') and return to the terminal. You can get a list of current jobs by using the `jobs` command. To return to the program you just supended, use the `fg` (for 'foreground') command. If you have suspended multiple jobs, use `fg <number>` to return to the job with number `<number>`. This number is seen when using the `jobs` command.

You can also execute suspended jobs in the background by using the command `bg` or `bg <number>`. If the job in question is waiting for input from STDIN (as an editor or an REPL are), then it will be automatically suspended immediately after executing it. However, you can e.g. run a script in the background in this way.


## Exercises

### Task 1

- Windows: To run the `qemu-system-aarch64` command, it may be necessary to add the QEMU install path to your `PATH` environment variable ('miljøvariabel').

- Windows: I experience issues with running the VM in either the Command Prompt or PowerShell on Windows. There is a strange visual bug I haven't been able to solve. Luckily, when connecting via SSH from another terminal window the bug is not present in the other window.

- Windows: In PowerShell, you can use the command `cat command.txt | clip` to copy the contents of the `command.txt` to the clipboard.

- To shut down the VM, use the command `sudo poweroff`.


### Task 3

If you do not already have an SSH client, then OpenSSH is the recommended option. On Windows you may again need to add the install path to your `PATH` environment variable to use the `ssh` command in a terminal window.


## Hand-in

No hand-in this week.


## Further reading

### Linux VM

If you don't have your own Linux VM and wish to have one (which is not necessary for the course, but may be nice to have), [this guide](https://www.theodinproject.com/lessons/foundations-installations#os-installation) from The Odin Project is a friendly walkthrough of how to install a virtual machine using VirtualBox. The Odin Project uses Xubuntu, a more lightweight but still very beginner-friendly derivative of Ubuntu, but use any distribution of Linux you are comfortable with.


### Version control

Since you will be working in groups, it might be useful to use some sort of version control. Git (with hosting on GitHub) is the obvious choice. The Odin Project also has a guide on how to [set up Git](https://www.theodinproject.com/lessons/foundations-setting-up-git). If you want to brush up on your Git, there are also guides available [here](https://www.theodinproject.com/paths/foundations/courses/foundations#git-basics).


### LaTeX

You may of course type up your hand-ins however you wish, but LaTeX is obviously the recommended option. [Overleaf](https://www.overleaf.com/) allows you to collaborate in real-time on a single LaTeX document.


### SSH

For more on SSH, I recommend *SSH, The Secure Shell: The Definitive Guide*, by Barrett, Silverman and Byrnes, especially chapters 1, 2 and 6 (and chapter 3 for the enthusiast).

Note that the list of public SSH keys on GitHub located in Settings only shows the SHA256 fingerprints of the keys. The keys themselves can be found at

    https://github.com/<username>.keys

For example, my keys are located [here](https://github.com/dnhansen.keys). (Of course, the corresponding *private* keys should never leave your machine!)


### Vim

If you wish to use Vim, then the `raspberrypi` VM (and most other Linux installations) comes equipped with the `vimtutor` command, which is a hands-on crash course in Vim. You can run Vim in the terminal by using the `vim` command.