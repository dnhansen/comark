# Week 4 (calendar week 8)

## General notes

### Logical and arithmetic shifts

The exercise sheet defines **logical shifts** of binary strings. A left shift inserts a zero to the right of the string and discards the left-most bit. A right shift inserts a zero to the left of the string and discards the right-most bit. In effect, a left shift multiplies by two and a right shift divides by two, as long as the binary string is interpreted as an *unsigned* number.

However, just as with sign extension, we must be careful when shifting binary strings when these are interpreted as *signed* numbers. Left shifts work the same, but we introduce another kind of right shift: An **arithmetic right shift** instead inserts *either* a one or a zero to the left of the string and discards the right-most bit. If the original left-most bit is zero then a zero is inserted, and if it is one then a one is inserted. To see that this indeed does divide the corresponding signed number by two, notice that if the left-most bit of a binary string is one, then an arithmetic right shift is equivalent to taking the two's complement of the string, performing a *logical* right shift, and taking the two's complement of the result. This changes sign (which results in a binary string whose value as a signed number is positive), divides by two, and changes sign again.

Both ARM and x68 assembly have instructions for both logical and arithmetic right shift.


## Exercises

This week has a lot of tasks, but many of them are very short.

### Chocolate Challenges

If you can solve a Chocolate Challenge, you may be entered into a raffle to win a bar of chocolate! If you think you have a solution to a Chocolate Challenge, write it up and send it to me.

Note that Chocolate Challenges are *completely optional*, and the material covered in them is not curriculum.

### 1. Seeing Assembly in Action

The purpose of these exercises is *not* to learn to understand ARM assembly. The purpose is to familiarise you with the overall appearance of assembly, and for you to realise that there is nothing to be afraid of! Don't spend too much time on these exercises.

### 2. A Basic Made-up Assembly

In these exercises we start actually programming in assembly. We use a toy assembly language which resembles ARM assembly, so we cannot actually run our code. The purpose of these exercises is to practice solving problems using only very basic instructions. These exercises are probably the most important.

- The `SHR` instruction is a right *logical* shift. If it had been a right *arithmetic* shift, then some of the exercises could not be solved (at least as far as I can tell).

- You *cannot* use registers other than `R6` and `R7` as scratch registers (and do not need to, obviously). Different kinds of assembly languages use different kinds of so-called **calling conventions** which, among other things, specify whether it is the caller or the callee of a function who has the responsibility of saving the values of different registers. (If a register is e.g. caller-saved, then it is the caller's responsibility to save the content of the register if it is to be used later, and the callee can use the register without backing up the content.) In ordinary assembly programming we usually have access to some kind of external memory, in which case we could write the contents of a callee-saved register to memory, use the register as a scratch register, and restore the value from memory before returning. Accessing memory is very expensive compared to reading from and writing to registers, so we like to keep it to a minimum!
  
  While we won't be calling any functions in these exercises (indeed, our toy assembly language does not even support functions in the first place), this is idea behind using only some registers as scratch registers.

### 3. Branch Prediction Experiments

These exercises are the least important this week. It is not the end of the world if you don't have time for them.


## Hand-in

### Task 1

There is a rather elegant solution to this task (see task 2), but the obvious solution (checking cases) also works.


### Task 2

Be careful! In the previous task `R7 = 1` when `R0` and `R1` are consecutive, but in this task we instead require that `R7 = 0` when `R0` and `R1` are consecutive. As far as I can tell, there is no reason for this discrepancy.

This may be slightly tricky. If you need a hint, notice that `R0` and `R1` contain consecutive integers if and only if `R0 - R1` is either 1 or -1, if and only if `(R0 - R1) + 1` is either 0 or 2.


### Task 3

I'm sure there are many solutions to this task. The one I have in mind requires realising the following, for two $k$-bit numbers $a$ and $b$:

1. If adding $a$ and $b$ them produces carry, then that means that the result has been truncated, i.e. subtracted by $2^k$. What can we say about the size of this truncated sum in comparison to $a$ and $b$? What if there is *no* carry?

2. We have a way of figuring out which of $a$ and $b$ is greater.