# C notes

http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf

## Undefined and unspecified behaviour

C, and most (all?) widely used programming languages in use, have a **specification**. This describes the behaviour that a C language construct must exhibit under specific circumstances. If the specification does not prescribe that a program must behave in a specific way, then the behaviour of that program is said to be **unspecified**.

On the other hand, the language specification may explicitly prescribe the behaviour to be unpredictable, in which case it is called **undefined**.


## Conversion specifications

When formatting a string in C using `printf`, we use **conversion specifications**, which are strings that begin with the character `%` and are terminated by a **conversion specifier**: The C standard provides a list of legal conversion specifiers, the most common being `d` (for 'digit'), `u` (for 'unsigned'), `f` (for 'float'), and so on. The conversion specifier can be preceded by an optional **length modifier**, e.g. `l` (for long) or `z` (used when printing values of type `size_t`). The standard also describes the behaviour that the `printf` function must have when a specific conversion specifier is used with an argument of a particular type. However, the standard also prescribes that if the wrong type of argument is used, then the behaviour is undefined.

The behaviour of a program in which there is a mismatch between a conversion specification and the type of an argument is thus entirely implementation dependent. Of course, any sensible implementation will do something sensible: Either give an error (on x86-64, `gcc` gives a warning when run with the option [TODO find the right option], but compilation is successful anyway), or give some sort of sensible output. If a value of type `long int` is given as argument and the corresponding conversion specification is e.g. `%d`, then it of course makes sense to print the integer anyway if it fits into an `int`.


## Integer conversions

The C standard describes the behaviour of a program when attempting to convert between different types of signed or unsigned integers. From the C99 standard:

> 6.3.1.3 Signed and unsigned integers
>
> 1. When a value with integer type is converted to another integer type other than _Bool, if the value can be represented by the new type, it is unchanged.
>
> 2. Otherwise, if the new type is unsigned, the value is converted by repeatedly adding or subtracting one more than the maximum value that can be represented in the new type until the value is in the range of the new type.
>
> 3. Otherwise, the new type is signed and the value cannot be represented in it; either the result is implementation-defined or an implementation-defined signal is raised.

Put simply: If the concrete value in question can be represented by the new type, it is unchanged. Otherwise, if the new type is unsigned then conversion is done by division with remainder. Otherwise the new type is signed, and the behaviour is implementation-dependent.


## `sizeof` and `size_t`

The keyword `sizeof` yields the size (in bytes) of its argument. *By definition*, the type of the result is `size_t` (from the `stddef.h` header file). This is an unsigned integer type used to represent the *size* of objects (so not objects themselves), for instance arrays. If we instead used e.g. `unsigned int` we might get in trouble if `int`s are 32 bits long but you are on a 64-bit machine and wish to work with objects whose size is greater than $2^{32}$. This is apparently a common problem when porting programs from 32-bit to 64-bit.

Since `size_t` is its own type, it also has its own conversion specification, namely `%zu`. Here `z` is the length modifier corresponding to `size_t`, and `u` of course indicates that the value is unsigned. There is also a *signed* type corresponding to the unsigned `size_t` (with conversion specification `%zd`), but this does not get a name. A type called `ssize_t` is defined in some header files, and this is somehow supposed to be the signed version of `size_t`, but whether it is or not is implementation-dependent.

As mentioned above, a mismatch between a conversion specification and the type of the argument results in undefined behaviour. I am actually unsure whether using other conversion specifications than `%zu` is valid, as long as the specification we use corresponds to a type that is at least as large as `size_t`. Some comments on StackOverflow indicate that, at least in practice, `size_t` is just a `typedef` of some other integer type, but I don't think the specification says that it must be so.


## Memory allocation

The memory used by a C program is often said to be divided into two: the *stack*, which is used for local variables and function arguments, and the *heap*, on which we can allocate memory dynamically, perhaps if we wish to store an array larger than can fit on a stack. Running `ulimit -a` in a Bash terminal returns user limits, such as the maximum size of a program stack. Note that there is no obvious connection between *the* heap, the place in memory, and *a* heap, the data structure. This is confusing since *the* stack is certainly *a* stack.

However, *nothing* in the C standard requires there to even be a stack or a heap anywhere. In fact, local variables are often placed in registers by optimising compilers instead of being placed on the stack.

In practice, the stack is usually used more or less as in the IJVM memory model: When a function is called, a block of memory is reserved on top of the stack for local variables and other information, and this block is popped when the function returns. In contrast, the heap is indeed usually used for dynamic allocation.

## Type system

In C, *variables* have types but *values* do not. Compare e.g. Python where variables are just names, but values (i.e. objects) have types (i.e. belong to classes). When a variable is declared, for instance by writing

    int n;

then the compiler understands that the name `n` refers to a value of type `int`. The compiler can use this information in the usual ways to ensure that the correct types are given as arguments to functions, to implement random access into arrays, etc.

At some point the variable will likely be associated with some value located either in memory or in a register. But the bits at this location do not contain information about the *type* of information stored there, nor is this information stored anywhere else. In particular there is no way to check the type of a variable at runtime, types are only available at compile-time.

(Strictly speaking there is, to my understanding, nothing in the standard that prohibits variables from having types at runtime. But then again, the standard of course includes no function to check the type of a variable, so one would need to provide this on top of the functions given in the standard.) <!-- TODO? -->


## Functions

### Function prototypes

Depending on the compiler, you might be able to call a function before it is declared, but the compiler is required by the C11 [TODO: Already in C99?] standard to complain in some way that this is happening. So why not just declare (and simultaneously define) every function before it is used? Sometimes this is not possible, for instance if two functions are mutually recursive.

The solution is to use *function prototypes*, which is a declaration of the function's return and argument types. If this is placed at the top of the file, the functions can be used when later *defining* each function. Function prototypes are recommended whether or not they are strictly necessary.


### Integer promotion in variadic functions

A C function that takes a variable number of arguments (e.g. `printf`) is usually called **variadic** (this term does not appear in the specification of C, but is standard nonetheless). If a value of type `char` or `short` is given as argument to a variadic function, then the value undergoes **integer promotion**: This means that it is converted to a value of type `int`. This also happens in various other circumstances in C programs, for instance in arithmetic operations. (In fact, the exact same phenomenon occurs in Java, where the type that values are converted to is known as the **promotion type**.)

This means that the length modifiers `h` and `hh` in `printf` cannot reasonably expect the corresponding argument to be of type `short` and `signed char` respectively (let's say we use the conversion specifier `d`, so the value is signed), since the argument will have been converted to an integer. Hence the value is converted to a `short` or a `signed char` before printing.


## Pointers

### Pointer types

Note that a pointer is not just a pointer, it is a pointer *to something*.

### Printing pointers

Note that the conversion specifier `p` expects a value of type `void*` (see C11, 7.21.6.1, p. 8). Strictly speaking, giving as argument to `printf` another type of pointer, e.g. `int*`, results in undefined behaviour, and hence is implementation-dependent. A warning is produced if compiled with the flag `-Wpedantic`. So, again strictly speaking, casting to `void*` is required but it is not usually necessary in practice. (Note, however, that casting to and from `void*` more generally is done implicitly.)

The reason this is necessary is that the C standard does not require that pointer types have the same size or representation.(What is true is that pointers to *compatible* types have the same representation, see C11, 6.2.5 p. 28.) In practice a `void*` and an `int*` have the same size (for instance 64 bits on a 64-bit machine) and are represented in the same way, but the standard does not require this, and there is no guarantee that this will not change in the future. And even if they were practically identical, there is no guarantee that the compiler will treat them the same.

### Double pointers

Since a pointer has a memory address, one can also consider pointers to pointers, denoted by two asterisks `**`. But there is nothing special about these objects, they are just pointers to a type that happens to itself be a pointer type.

### Incomplete types

[TODO]


## Structs

We distinguish between *defining* and *declaring* structs. The minimal syntax for defining a struct is

    struct {};

This defines a struct with no name, so it cannot be used subsequently. There is really no reason to do this. We may also give the struct a name, e.g.

    struct s {};

which defines a struct with the name `s`. To declare a variable of this type we write

    struct s t;

We may also use the struct definition immediately when declaring a variable:

    struct {} t;

In this case it can make sense not to give the struct a name, but this precludes us from declaring other variables of the same type. We may instead give the struct a name *and* declare the variable `t` with the corresponding type in the same declaration:

    struct s {} t;

The braces in all of the above cases may of course contain variable declarations and so on as usual. Members of incomplete types are not allowed.

After the struct has been named it has incomplete type: it is only when the entire struct definition is complete that it has complete type. Since the variable declarations inside a struct definition do not allow incomplete types, the following is *not* allowed:

    struct s { struct s t; };

However, pointers to incomplete types \emph{are} allowed, so the following is legal:

    struct s { struct s *p; };

Hence it is possible to create (effectively) recursive structs.

Instead of defining a struct we may also \emph{declare} it. A declaration on the form

    struct s;

is called a *forward declaration*. This declares `s` as a new struct name in the scope (hiding any previously declared meaning for the name), but it does not define it. Until it has been defined, using the syntax above, the struct name has incomplete type. This syntax is useful in defining mutually recursive structs, e.g.

    struct r;
    struct s { struct r *p; };
    struct r { struct s *q; };

Notice that the definition of the struct `s` is legal since *pointers* to incomplete types are legal in struct definitions. Compare also function prototypes mentioned above.


## The POSIX Thread API

### Passing arguments to threads

Recall the signature of the [`pthread_create`(3)](https://man7.org/linux/man-pages/man3/pthread_create.3.html) function:

    int pthread_create(pthread_t *restrict thread,
                       const pthread_attr_t *restrict attr,
                       void *(*start_routine)(void *),
                       void *restrict arg);

The function `start_routine` thus takes as its only parameter an object of type `void*`, which it passed through the `arg` parameter.


#### Naive conversion to `void*`

If we wish to e.g. pass an integer `i` to `start_routine`, it is tempting to pass a pointer `&i`. This may be fine, but one has to be aware of the following:

- If it is allocated on the stack, it will be freed when the calling thread terminates.
- If `&i` is passed to multiple threads, then every such thread may modify the value located at the address `&i`. This is usually not the intended behaviour.

Another approach is to simply cast `i` to a `void*` and pass this. The C standard e.g. allows integer constant expressions with value `0` to be cast to `void*` to obtain a **null pointer** (C11, 6.3.2.3, p.3). However, apart from this things are less clear:

> An integer may be converted to any pointer type. Except as previously specified, the result is implementation-defined, might not be correctly aligned, might not point to an entity of the referenced type, and might be a trap representation. (C11, 6.3.2.3, p.5)

And conversely:

> Any pointer type may be converted to an integer type. Except as previously specified, the result is implementation-defined. (C11, 6.3.2.3, p.6)

We run into implementation-defined behaviour, which we would like to avoid.


#### Using `intptr_t`

Another common attempt is to use the signed integer type `intptr_t` and its unsigned counterpart `uintptr_t` defined in the header file `<stdint.h>`. The standard (C11, 7.20.2.4) specifies that `intptr_t` must be able to contain values between $-(2^{15}-1)$ to $2^{15}-1$ (the value $-2^{15}$ is not required to accomodate e.g. one's complement representation of signed integers), and `uintptr_t` must be able to contain values up to $2^{16}-1$.

The standard futhermore specifies the following:

> [`intptr_t`] designates a signed integer type with the property that any valid pointer to `void` can be converted to this type, then converted back to pointer to `void`, and the result will compare equal to the original pointer. (C11, 7.20.1.4)

So the standard at least gives us some guarantees when converting to and from `void` pointers. We would then perform a cast like

    (void*)(intptr_t)i

and pass the result as the `arg` parameter of `pthread_create`.

However, we run into the following problems:

- The types `intptr_t` and `uintptr_t` are optional (C11, 7.20.1.4). So there are no guarantees in case we want to write portable code.
- The standard only specifies the *minimum* size of the type, namely as 16-bit integers. Since we are able to perform a conversion `void*` -> `intptr_t` -> `void*` without losing information, `intptr_t` must be able to contain every pointer to `void`. However, the standard gives us *no* guarantee that the converse is the case, i.e. that we can perform a conversion `intptr_t` -> `void*` -> `intptr_t`. In other words, `intptr_t` may be a much larger type than `void*`. But notice that this latter conversion is precisely the one we wish to perform!
- We would like to convert from `intptr_t` to some concrete integer type, e.g. to print it. Since there is no guaranteed upper bound on the size of `intptr_t`, this is difficult. [Recall](#integer-conversions) that converting to signed integer types can be problematic, and at best converting to unsigned integer types can lose information if they are not large enough. This is less of a problem in our use case since we of course know the original type of the argument and can write `start_routine` accordingly, but it is one more thing to be careful of.

In short, this approach is less than ideal.


#### Carefully passing pointers

TODO converting to and from `void*`.

The usual approach is to somehow (either on the stack or on the heap) allocate memory for the object to be passed, and then being very careful about not passing the same pointer to multiple threads. If one e.g. has a loop that creates threads, one may first create an array to contain the arguments to each thread, and then iterate through this array.

If the arguments are allocated on the stack, one must (as mentioned above) be very careful that the parent thread does not exit before the child does. There are various ways of ensuring this (e.g. using semaphores), but the POSIX Thread API provides the `pthread_join` function which may be used for this purpose.

See TODO for a working example of this.


### Passing multiple arguments

The function `start_routine` only has one parameter, so we cannot immediately pass multiple arguments to it. Of course this should be no hindrance. After all, mathematical functions (at least in the usual conception of functions in mathematics) only take one argument, but that argument may be a tuple.

The usual way of accomplishing this is by declaring a struct to hold the arguments. For instance,

    typedef struct args_st {
        int arg1;
        int arg2;
    } args_t;

defines the type `args_t` as a struct type with fields `arg1` and `arg2` of type `int`. One may then allocate such structs and pass pointers to them to `pthread_create`. Each child can then free the memory themselves.
