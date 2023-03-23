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


### Pointer arithmetic

We may perform arithmetic with pointers. First of all, the standard has the following to say about addition:

> For addition, either both operands shall have arithmetic type, or one operand shall be a pointer to a complete object type and the other shall have integer type. (Incrementing is equivalent to adding 1.) (C11, 6.5.6, p.2)

As for what 'complete' object types are:

> Types are partitioned into *object types* (types that describe objects) and *function types* (types that describe functions). At various points within a translation unit an object type may be *incomplete* (lacking sufficient information to determine the size of objects of that type) or *complete* (having sufficient information). (C11, 6.2.5, p.1

> The `void` type comprises an empty set of values; it is an incomplete object type that cannot be completed. (C11, 6.2.5, p.19)

That is, it is *not* possible to perform addition with pointers of type `void*`, since these are pointers to incomplete types. As for subtraction:

> For subtraction, one of the following shall hold:
> 
> - both operands have arithmetic type;
> - both operands are pointers to qualified or unqualified versions of compatible complete object types; or
> - the left operand is a pointer to a complete object type and the right operand has integer type.
> 
> (Decrementing is equivalent to subtracting 1.) (C11, 6.5.6, p.3)

[TODO qualified, unqualified]

Again we require the pointer(s) to be (a) pointer(s) to (a) complete type(s), so again it is impossible to do subtraction with `void`-pointers.

As for the semantics of pointer arithmetic:

> When an expression that has integer type is added to or subtracted from a pointer, the result has the type of the pointer operand. If the pointer operand points to an element of an array object, and the array is large enough, the result points to an element offset from the original element such that the difference of the subscripts of the resulting and original array elements equals the integer expression. \[...\] If both the pointer operand and the result point to elements of the same array object, or one past the last element of the array object, the evaluation shall not produce an overflow; otherwise, the behavior is undefined. (C11, 6.5.6, p.8)

In other words, it is only possible to add an integer to or subtract an integer from a pointer if the pointer points into an array, and the resulting pointer doesn't take us 'out of' the array. And when this is done, the amount the pointer is incremented by is determined by the type of the elements in the array. If two pointers point into the same array, we may also subtract them:

> When two pointers are subtracted, both shall point to elements of the same array object, or one past the last element of the array object; the result is the difference of the subscripts of the two array elements. The size of the result is implementation-defined, and its type (a signed integer type) is `ptrdiff_t` defined in the `<stddef.h>` header. If the result is not representable in an object of that type, the behavior is undefined. (C11, 6.5.6, p.9)


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

Array-pointer equivalence tells us that `a[2] == *(a + 2)` and supposedly also that `2[a] == *(2 + a)`. But as we saw above, this is permitted by the C standard.


### Pointers to functions

TODO
