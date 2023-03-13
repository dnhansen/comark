# Week 6 (calendar week 10)

## Exercises

Note that there are type mismatchs in the program `codes/fac.c`: Both `a` and `r` are declared as `long`s which are *signed*, but the conversion specifiers used for them in the first argument to `printf` are in both cases `u`, which are used with *unsigned* integers. Strictly speaking the behaviour of the program is thus undefined, but of course it still works in practice (as we saw in week 2). I recommend using the specifier `d` for signed integer types (with appropriate length modifiers of course).

ARM uses what is knows as a **full descending stack**. 'Descending' refers to the fact that the stack grows downwards, i.e. pushing to the stack decrements the stack pointer, and popping from the stack increments the stack pointer. 'Full' refers to the fact that the stack pointer contains the address of the topmost (or bottommost, depending on how you look at it) element of the stack. On some architectures, the stack pointer contains the address just beyond the top of the stack, i.e. the address you would write to if you wanted to push to the stack.