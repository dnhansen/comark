# Week 3 (calendar week 7)

## General notes

### Modular arithmetic

See my [notes](https://github.com/dnhansen/comark/blob/main/arithmetic-notes.md) on modular arithmetic and its use in computer arithmetic.


### Fetch-execute cycle

In Algorithm 5.1 there is an explicit distinction between the **program counter** `PC` and the **(memory) address register**, often called `MAR`. Different architectures treat these registers in different ways, but `PC` generally stores the address of the next instruction to be fetched, and `MAR` is a more general purpose register for storing the address of data in memory. In the book's example architecture, we are able to use the contents of `PC` directly to fetch the next instruction from memory, but in some architectures we would need to move the contents of `PC` to `MAR` before fetching, simply because `PC` isn't directly connected to memory in these architectures.

Note that `PC` just needs to be updated before the end of the fetch-execute cycle. In this example it is updated by copying the contents of `MAR` to `PC` just before the cycle ends, but we could also imagine incrementing it immediately after fetching.


## Exercises

## Hand-in

Deadline: Friday the 17th February, 11:59 p.m.

### Task 1

A combinatorical circuit that implements the incrementation operation is known as an **incrementer**.

Don't worry about the 'places the content in the register R2' part of this task. Just design a combinatorial circuit that performs the desired operation, namely adding 1 to a binary number.

Note that by using *n* full adders we can actually implement a circuit that adds any two *n*-bit numbers (and outputs a potential carry bit). If this task seems easier, then design a circuit that can add two binary numbers and use it to add 1 to the input.

It is interesting to note that we can in fact design an incrementer using *n* *half* adders. Since the task specifically asks you to use *full* adders, I will not accept solutions that use half adders.

Since a full adder is a relatively complicated circuit, you do not need to reproduce an implementation of it in your solution. Just represent each adder with a box. The standard symbol for a full adder is a box labeled with a Greek uppercase 'Σ', having on one side each input bit (of which there are 2*n*+1, since the adder adds two *n*-bit numbers, and the first adder also has a carry-in), and on the other side each output bit (*n*+1 bits, including the carry-out). But as long as your drawings are clear, you do not need to represent full adders in any particular way. Since it is of course impossible to draw such a diagram for an arbitrary *n*, just show how the 0th, 1st, and (*n*-1)th adders are connected.


### Task 2

You of course need to argue that your chosen implementation of the XOR gate is in fact the most cost-efficient! You do not need to prove that there does not exist an even more cost-efficient implementation, it suffices to argue that it is at least as efficient as the other two implementations in Figure 3-8. 


### Task 5

This task is optional, but I am of course very happy to correct any solutions! Solutions to this task (or any optional tasks in the future) will not be taken into account when I consider whether or not to accept your solutions.


## Further reading

For more on modular arithmetic, mathematics students at AU use the book

1. Hansen, *Tal og polynomier*. Written by the late Johan P. Hansen, it is used in introductory algebra courses at AU. It contains a gentle but rigorous introduction to modular arithmetic and basic group and ring theory, as well as many applications of basic number theory, such as RSA encryption and error-correcting codes.
