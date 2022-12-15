# C notes

http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1548.pdf

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

### Argument promotion

Arguments to a function with a prototype are implicitly converted to the types of the corresponding parameters. If the function does not have a prototype, the *default argument promotions* are used, in which expressions of type `char` or `short int` are automatically promoted to `int`.

### Variadic functions

Some functions take a variable number of arguments, for instance `printf`. These are usually called *variadic* functions, though this terminology is not found in the C11 standard. According to [*The GNU C Library Reference Manual*](https://www.gnu.org/software/libc/manual/html_node/Variadic-Prototypes.html), a variadic function must be declared with a prototype that says that it is variadic, which is done with an ellipsis '`...`' appearing last in the parameter list, following at least one named parameter. Arguments corresponding to the ellipsis seem to be referred to as *variadic arguments*, but this also does not seem to be an official term.

Arguments corresponding to the named parameters of a variadic function (which has a prototype) are implicitly converted as with non-variadic functions, and the variadic arguments undergo default argument promotion (C11, 6.5.2.2, p. 7).

### The function `sizeof`
The function `sizeof` has the return type `size_t`, which is specifically used to represent the size of objects. It is guaranteed to be large enough to contain the size of the largest object the system can handle.

If we wish to print a value of type `size_t`, we therefore use the length modifier `z` along with the conversion specifier `u`, since the value is unsigned. This only works if the compiler supports C99 (which it almost certainly does), but if it doesn't then `lu` also works. The compiler won't complain, but it is probably good style to explicitly cast to `unsigned long` before printing. (Actually, on some systems it might be necessary to use `llu`, if an `unsigned long` is not big enough to represent the largest object for the given environment.)


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

After the struct has been named it has incomplete type: it is only when the entire struct definition is complete that it has complete type. Since the variable declarations inside a struct definition do not allow incomplete types, the following is \emph{not} allowed:

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