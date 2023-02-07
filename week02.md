# Week 2 (calendar week 6)

## General notes

### Undefined and unspecified behaviour

C, and most (all?) widely used programming languages in use, have a **specification**. This describes the behaviour that a C language construct must exhibit under specific circumstances. If the specification does not prescribe that a program must behave in a specific way, then the behaviour of that program is said to be **unspecified**.

On the other hand, the language specification may explicitly prescribe the behaviour to be unpredictable, in which case it is called **undefined**.

When formatting a string in C using `printf`, we use **conversion specifications**, which are strings that begin with the character `%` and are terminated by a **conversion specifier**: The C standard provides a list of legal conversion specifiers, the most common being `d` (for 'digit'), `u` (for 'unsigned'), `f` (for 'float'), and so on. The conversion specifier can be preceded by an optional **length modifier**, e.g. `l` (for long) or `z` (used when printing values of type `size_t`). The standard also describes the behaviour that the `printf` function must have when a specific conversion specifier is used with an argument of a particular type. However, the standard also prescribes that if the wrong type of argument is used, then the behaviour is undefined.

The behaviour of a program in which there is a mismatch between a conversion specification and the type of an argument is thus entirely implementation dependent. Of course, any sensible implementation will do something sensible: Either give an error (on x86-64, `gcc` gives a warning when run with the option [TODO find the right option], but compilation is successful anyway), or give some sort of sensible output. If a value of type `long int` is given as argument and the corresponding conversion specification is e.g. `%d`, then it of course makes sense to print the integer anyway if it fits into an `int`.


### Integer promotion in variadic functions

A C function that takes a variable number of arguments (e.g. `printf`) is usually called **variadic** (this term does not appear in the specification of C, but is standard nonetheless). If a value of type `char` or `short` is given as argument to a variadic function, then the value undergoes **integer promotion**: This means that it is converted to a value of type `int`. This also happens in various other circumstances in C programs, for instance in arithmetic operations. (In fact, the exact same phenomenon occurs in Java, where the type that values are converted to is known as the **promotion type**.)

This means that the length modifiers `h` and `hh` in `printf` cannot reasonably expect the corresponding argument to be of type `short` and `signed char` respectively (let's say we use the conversion specifier `d`, so the value is signed), since the argument will have been converted to an integer. Hence the value is converted to a `short` or a `signed char` before printing.


### Integer conversions

The C standard describes the behaviour of a program when attempting to convert between different types of signed or unsigned integers. From the C99 standard:

> 6.3.1.3 Signed and unsigned integers
>
> 1. When a value with integer type is converted to another integer type other than _Bool, if the value can be represented by the new type, it is unchanged.
>
> 2. Otherwise, if the new type is unsigned, the value is converted by repeatedly adding or subtracting one more than the maximum value that can be represented in the new type until the value is in the range of the new type.
>
> 3. Otherwise, the new type is signed and the value cannot be represented in it; either the result is implementation-defined or an implementation-defined signal is raised.

Put simply: If the concrete value in question can be represented by the new type, it is unchanged. Otherwise, if the new type is unsigned then conversion is done by division with remainder. Otherwise the new type is signed, and the behaviour is implementation-dependent.


### `sizeof` and `size_t`

The keyword `sizeof` yields the size (in bytes) of its argument. *By definition*, the type of the result is `size_t` (from the `stddef.h` header file). This is an unsigned integer type used to represent the *size* of objects (so not objects themselves), for instance arrays. If we instead used e.g. `unsigned int` we might get in trouble if `int`s are 32 bits long but you are on a 64-bit machine and wish to work with objects whose size is greater than $2^{32}$. This is apparently a common problem when porting programs from 32-bit to 64-bit.

Since `size_t` is its own type, it also has its own conversion specification, namely `%zu`. Here `z` is the length modifier corresponding to `size_t`, and `u` of course indicates that the value is unsigned. There is also a *signed* type corresponding to the unsigned `size_t` (with conversion specification `%zd`), but this does not get a name. A type called `ssize_t` is defined in some header files, and this is somehow supposed to be the signed version of `size_t`, but whether it is or not is implementation-dependent.

As mentioned above, a mismatch between a conversion specification and the type of the argument results in undefined behaviour. I am actually unsure whether using other conversion specifications than `%zu` is valid, as long as the specification we use corresponds to a type that is at least as large as `size_t`. Some comments on StackOverflow indicate that, at least in practice, `size_t` is just a `typedef` of some other integer type, but I don't think the specification says that it must be so.


## Exercises

### General notes

Many of the details on the exercise sheet are wrong (compare my notes above), so don't read too much into them. In general, do not expect to have a deep understanding of C after doing the exercises. This is not the point, and indeed it is impossible.

Do not hesitate to ask if you are interested in knowing more about C!


### Task 3

You do not need to copy the hex-to-binary conversion table. Also note that the column of hex-values in fact lists *decimal* values, which is an error.


## Hand-in

No hand-in this week.


## Further reading

The classic reference on C is

1. Kernighan, Ritchie, *The C Programming Language*. It's easy enough to read, but I'm not sure it's the best way to learn C these days. Many of the exercises are particularly tedious.

My own favourite book is

2. Hall, *Beej's Guide to C Programming*. It is freely available from [Hall's website](https://beej.us/guide/bgc/). It's rather talkative and tries a bit too hard to be funny, but it covers all the major topics. Sadly it does not contain any exercises.

There are various revisions of the C programming language. The latest stable release is known as C17 (or C18), and its specification can be found [here (pdf)](https://files.lhmouse.com/standards/ISO%20C%20N2176.pdf). The next major revision is usually known as C2x and is expected to come out in 2023 (thus gaining the name C23). The latest working draft of the specification of C2x can be found [here (pdf)](https://open-std.org/JTC1/SC22/WG14/www/docs/n3088.pdf).