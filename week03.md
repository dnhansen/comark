# Week 3 (calendar week 7)

## General notes

### Implementing addition

We wish to design a combinatorial circuit that allows us to perform arithmetic operations. Such a circuit will have a series of binary inputs and outputs, and it is clearly possible to perform operations on larger numbers if the number of inputs and outputs is greater. However, it is also clear that we cannot feasibly build arbitrarily large circuits, so we settle on some fixed input size $k$. Since the arithmetic circuit is supposed to perform *binary* operations like addition, it will have two $k$-bit inputs and one $k$-bit output.

Before considering how to represent actual numbers using bits, we first consider which numbers we wish to represent. We know that if two unsigned integers are large enough, when e.g. a C program computes their sum the result will be wrong. And it seems a bit strange to use binary strings to represent numbers if this results in operations that are clearly wrong.

Hence we instead use binary strings to represent *residue classes* of numbers. This choice will turn out to make arithmetic operations performed on a computer make sense. Furthermore, since there are $2^k$ possible binary strings of length $k$, it seems reasonable to associate to each such string a congruence class modulo $2^k$, i.e. an element of $\mathbb{Z}/2^{k}\mathbb{Z}$.

Before proceeding, we introduce some notation: If $[n]$ is a residue class modulo $2^k$, then we denote by $\mathrm{unsigned}([n])$ the unique residue $r \in [0,2^{k}-1]$ such that $[r] = [n]$. We call this the **unsigned interpretation** of $[n]$. Similarly, we denote by $\mathrm{signed}([n])$ the unique residue $r \in [2^{k-1},2^{k-1}-1]$ such that $[r] = [n]$, and call this the **signed interpretation** of $[n]$.

Next we must decide on a correspondence between the set $\mathbb{Z}/2^{k}\mathbb{Z}$ of residue classes, and the set $\{0,1\}^k$ of binary strings of length $k$. Define a map $\mathrm{enc}\colon \mathbb{Z}/2^{k}\mathbb{Z} \to \{0,1\}^k$ by letting

$$ \mathrm{enc}([n]) = a_{k-1}a_{k-2} \cdots a_0, $$

where $a_{k-1}a_{k-2} \cdots a_1 a_0$ is the unique string of bits such that

$$ \mathrm{unsigned}([n]) = \sum_{i=0}^{k-1} a_i 2^{i}. $$

In other words, to compute $\mathrm{enc}([n])$ we first take the unsigned interpretation of $[n]$ and write this as a binary number. Then $\mathrm{enc}([n])$ is the string of binary digits of this number. It is clear that $\mathrm{enc}$ is a bijection, so we have our correspondence between residue classes and binary strings. Denote its inverse by $\mathrm{dec}$.

We now study the relationship between the addition on $\mathbb{Z}/2^{k}\mathbb{Z}$, and the addition performed by a $k$-bit ALU on binary strings. It should be clear from the usual addition algorithm that if $a_{k-1}a_{k-2} \cdots a_0$ and $b_{k-1}b_{k-2} \cdots b_0$ are binary strings, then the ALU computes their 'sum' as another binary string $c_{k-1}c_{k-2} \cdots c_0$ with the property that either

$$ \sum_{i=0}^{k-1} c_i 2^{i} = \sum_{i=0}^{k-1} a_i 2^{i} + \sum_{i=0}^{k-1} b_i 2^{i} $$

or

$$ \sum_{i=0}^{k-1} c_i 2^{i} = \sum_{i=0}^{k-1} a_i 2^{i} + \sum_{i=0}^{k-1} b_i 2^{i} - 2^{k}. $$

That is, if we interpret the binary strings as binary numbers, then the sum is either correct, or there has been a carry and the result is too small by the amount $2^k$. But in $\mathbb{Z}/2^{k}\mathbb{Z}$ this extra term $2^k$ does not matter, so it follows that

$$ \mathrm{dec}(c_{k-1}c_{k-2} \cdots c_0) = \mathrm{dec}(a_{k-1}a_{k-2} \cdots a_0) + \mathrm{dec}(b_{k-1}b_{k-2} \cdots b_0) $$

in $\mathbb{Z}/2^{k}\mathbb{Z}$. In other words, the addition on $\mathbb{Z}/2^{k}\mathbb{Z}$ is precisely the addition implemented by the ALU. This in particular induces an addition on $\{0,1\}^k$, which we simply denote by $+$.


### Additive inverses and subtraction

Next we wish subtract two residue classes $[n]$ and $[m]$. But instead of subtracting $[m]$ from $[n]$, we may simply *add* $[-m]$ to $[n]$. The only question is how to find a binary representation of $[-m]$ given a binary representation of $[m]$. But simply notice that since $[0] = [m] + [-m]$, we have

$$ \mathrm{enc}([0]) = \mathrm{enc}([m]) + \mathrm{enc}([-m]). $$

That is, $\mathrm{enc}([-m])$ is a binary string such that when the ALU adds it to $\mathrm{enc}([m])$, we get the binary string $00 \cdots 0$ (with $k$ zeros).

Consider a binary string $a_{k-1}a_{k-2} \cdots a_0$, and consider the **one's complement** of it, namely the string $\overline{a}_{k-1}\overline{a}_{k-2} \cdots \overline{a}_0$ where each bit has been flipped ($0$ changed to $1$ and vice-versa). Denote this by $\mathrm{comp}_1(a_{k-1}a_{k-2} \cdots a_0)$, and notice that

$$ a_{k-1}a_{k-2} \cdots a_0 + \mathrm{comp}_1(a_{k-1}a_{k-2} \cdots a_0) = (a_{k-1} + \overline{a}_{k-1})(a_{k-2} + \overline{a}_{k-2}) \cdots (\overline{a}_0 + a_0), $$

which is just the string $11 \cdots 1$ (with $k$ ones). Hence if we simply add $1$ to $\mathrm{comp}_1(a_{k-1}a_{k-2} \cdots a_0)$, we get precisely the string we want. We call the result the **two's complement** of $a_{k-1}a_{k-2} \cdots a_0$ and denote it by $\mathrm{comp}_2(a_{k-1}a_{k-2} \cdots a_0)$.

To sum up, this shows that to find $\mathrm{enc}([-m])$, we first take the encoding of $[m]$, and then take its two's complement, i.e.

$$ \mathrm{enc}([-m]) = \mathrm{comp}_2(\mathrm{enc}([m])). $$

The encoding of $[m]$ is easy to find, since it is almost certainly already stored in memory or in a register (since we wish to perform an operation on $[m]$, so it better be available to us!), and it is also easy to compute the two's complement of a binary string: Just flip all bits and add one. In practice, what usually happens is that all bits are flipped, and the lowest-order full adder in the ALU is given $1$ as a carry-in.

Put another way, $\mathrm{comp}_2(a_{k-1}a_{k-2} \cdots a_0)$ is the *additive inverse* of $a_{k-1}a_{k-2} \cdots a_0$ in $\{0,1\}^k$.


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