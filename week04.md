# Week 4 (calendar week 8)

## General notes

### Logical and arithmetic shifts

The exercise sheet defines **logical shifts** of binary strings. A left shift inserts a zero to the right of the string and discards the left-most bit. A right shift inserts a zero to the left of the string and discards the right-most bit. In effect, a left shift multiplies by two and a right shift divides by two, as long as the binary string is interpreted as an *unsigned* number.

However, just as with sign extension, we must be careful when shifting binary strings when these are interpreted as *signed* numbers. Left shifts work the same, but we introduce another kind of right shift: An **arithmetic right shift** instead inserts *either* a one or a zero to the left of the string and discards the right-most bit. If the original left-most bit is zero then a zero is inserted, and if it is one then a one is inserted. To see that this indeed does divide the corresponding signed number by two, notice that if the left-most bit of a binary string is one, then an arithmetic right shift is equivalent to taking the two's complement of the string, performing a *logical* right shift, and taking the two's complement of the result. This changes sign (which results in a binary string whose value as a signed number is positive), divides by two, and changes sign again.

Both ARM and x68 assembly have instructions for both logical and arithmetic right shift.


## Exercises

### Chocolate Challenges

If you can solve a Chocolate Challenge, you may be entered into a raffle to win a bar of chocolate! If you think you have a solution to a Chocolate Challenge, write it up and send it to me.

Note that Chocolate Challenges are *completely optional*, and the material covered in them is not curriculum.

### Assembly exercises

- The `SHR` instruction is a right *logical* shift. If it had been a right *arithmetic* shift, then some of the exercises could not be solves (at least as far as I can tell).

- You *cannot* use registers other than `R6` and `R7` as scratch registers (and do not need to, obviously). Different kinds of assembly languages use different kinds of so-called **calling conventions** which, among other things, specify whether it is the caller or the callee of a function who has the responsibility of saving the values of different registers. In ordinary assembly programming we usually have access to some kind of external memory, in which case we could write the contents of a callee-saved register to memory, use the register as a scratch register, and restore the value from memory before returning. Accessing memory is very expensive compared to reading from and writing to registers, so we like to keep it to a minimum!
  
  While we won't be calling any functions in these exercises (indeed, our toy assembly language does not even support functions in the first place), this is idea behind using only some registers as scratch registers.


## Hand-in

TODO


## Further reading

TODO