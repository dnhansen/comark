# Week 7 (calendar week 11)

## General notes

### Explicit conversion of pointers

Despite Peyman's claims, **explicit conversion** (also known as **casting**) of a pointer to a pointer of a different type is *not* always possible. For instance:

> A pointer to an object type may be converted to a pointer to a different object type. If the
resulting pointer is not correctly aligned for the referenced type, the behavior is
undefined. (C2011, 6.3.2.3, p.7)

More explicitly: Some systems have rules for how objects of different sizes can be stored in memory. For instance, it might be that a 32-bit integer (say an `int` in C) can only be stored on addresses that are a multiple of 4 (counting from 0). It might also be that a 16-bit integer (say a `short` in C) can only be stored on addresses that are a multiple of 2. But if we then store a `short` at address 2 (so that the integer is stored in bytes 2 and 3), obtain a pointer to it, and cast this pointer to a pointer to `int` and dereference, we obtain an `int` that is stored at address 2. But this is illegal, and the resulting program has undefined behaviour.

If one has a pointer `p` to e.g. a `short` and wishes to obtain a pointer `q` to an `int` such that `*p == *q`, one can use an intermediate object as follows:

    int x = *p;
    int *q = &x;

See e.g. [this page](https://wiki.sei.cmu.edu/confluence/display/c/EXP36-C.+Do+not+cast+pointers+into+more+strictly+aligned+pointer+types) for more examples.

On the other hand, it *is* always possible to convert to and from pointers to `void`:

> A pointer to `void` may be converted to or from a pointer to any object type. A pointer to
any object type may be converted to a pointer to `void` and back again; the result shall
compare equal to the original pointer. (C2011, 6.3.2.3, p.1)

Indeed this is necessary in order to write portable code, since the `%p` conversion specifier used in `printf` expects a pointer to `void`, and a pointer to any other type strictly speaking results in undefined behaviour.

This is also often used to obtain a primitive kind of polymorphism. See for instance the declaration of the `qsort` (quicksort) function from `stdlib.h`:

    void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void*, const void*));

The first parameter `base` points to the start of an array to be sorted, but since it has type `void*` there is no way to know how wide each element of the array is. Thus we also pass the size of each element as the parameter `size`. This allows the `qsort` function to iterate through the array.

Furthermore, the last parameter `compar` has the type "pointer to a function with return type `int` and parameters of type `const void*` and `const void*`. That is, `compar` takes two parameters, each of which are (constant) pointers to `void`. We may thus implement `compar` for different concrete types and reuse `qsort`.


### Array-pointer equivalence

Again despite Peyman's words, arrays and pointers are *not* the same, even if they often *behave* as though they are the same. They so often behave as though they are the same that we talk about **array-pointer equivalence**, by which we mean the (true) identity

    a[b] == *(a + b)

And indeed, arrays are often implicitly converted to pointers:

> Except when it is the operand of the `sizeof` operator or the unary `&` operator, or is a
string literal used to initialize an array, an expression that has type "array of *type*" is
converted to an expression with type "pointer to *type*" that points to the initial element of
the array object and is not an lvalue. (C2011, 6.3.2.1, p.3)

This is sometimes referred to as **array decay** or **array to pointer decay**. For an example of the difference between arrays and pointers, consider the following code:

    int a[10];
    int *b = a;
    printf("%zu\n", sizeof(a)); // prints "40"
    printf("%zu\n", sizeof(b)); // prints "8"

The array-pointer equivalence sometimes leads to surprising, but legal, programs. In the snippet

    int a[] = {0,1,2};
    int x = a[2];

obviously `x` will contain the value `2`. However, it turns out that the following is also possible:

    int a[] = {0,1,2};
    int x = 2[a];

Array-pointer equivalence tells us that `a[2] == *(a + 2)` and supposedly also that `2[a] == *(2 + a)`. Addition is only defined between two numbers, or between a number [TODO presumably only integers?] and a pointer. But notice that it is necessary for the compiler to know which of the operands `2` and `a` is a pointer, since it must add `2 * sizeof(int)` to `a` to obtain the address of the element in `a` with index `2`. Compare also pointer arithmetic, where we do not need to take the size of elements into account, and assembly, where we do.


### Pointers to functions

TODO
