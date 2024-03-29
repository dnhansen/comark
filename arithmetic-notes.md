# Notes on modular and computer arithmetic

## Implementing addition

We wish to design a combinatorial circuit that allows us to perform arithmetic operations. Such a circuit will have a series of binary inputs and outputs, and it is clearly possible to perform operations on larger numbers if the number of inputs and outputs is greater. However, it is also clear that we cannot feasibly build arbitrarily large circuits, so we settle on some fixed input size $k$. Since the arithmetic circuit is supposed to perform *binary* operations like addition, it will have two $k$-bit inputs and one $k$-bit output.

Before considering how to represent actual numbers using bits, we first consider which numbers we wish to represent. We know that if two unsigned integers are large enough, when e.g. a C program computes their sum the result will be wrong. And it seems a bit strange to use binary strings to represent numbers if this results in operations that are clearly wrong.

Hence we instead use binary strings to represent *residue classes* of numbers. This choice will turn out to make arithmetic operations performed on a computer make sense. Furthermore, since there are $2^k$ possible binary strings of length $k$, it seems reasonable to associate to each such string a congruence class modulo $2^k$, i.e. an element of $\mathbb{Z}/2^{k}\mathbb{Z}$.

Before proceeding, we introduce some notation: If $[n]$ is a residue class modulo $2^k$, then we denote by $\mathrm{unsigned}([n])$ the unique residue $r \in [0,2^{k}-1]$ such that $[r] = [n]$. We call this the **unsigned interpretation** of $[n]$. Similarly, we denote by $\mathrm{signed}([n])$ the unique residue $r \in [-2^{k-1},2^{k-1}-1]$ such that $[r] = [n]$, and call this the **signed interpretation** of $[n]$.

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


## Additive inverses and subtraction

Next we wish subtract two residue classes $[n]$ and $[m]$. But instead of subtracting $[m]$ from $[n]$, we may simply *add* $[-m]$ to $[n]$. The only question is how to find a binary representation of $[-m]$ given a binary representation of $[m]$. But simply notice that since $[0] = [m] + [-m]$, we have

$$ \mathrm{enc}([0]) = \mathrm{enc}([m]) + \mathrm{enc}([-m]). $$

That is, $\mathrm{enc}([-m])$ is a binary string such that when the ALU adds it to $\mathrm{enc}([m])$, we get the binary string $00 \cdots 0$ (with $k$ zeros).

Consider a binary string $a_{k-1}a_{k-2} \cdots a_0$, and consider the **one's complement** of it, namely the string $\overline{a}\_{k-1}\overline{a}\_{k-2} \cdots \overline{a}\_0$ where each bit has been flipped ($0$ changed to $1$ and vice-versa). Denote this by $\mathrm{comp}\_1(a\_{k-1}a\_{k-2} \cdots a\_0)$, and notice that

$$ a_{k-1}a_{k-2} \cdots a_0 + \mathrm{comp}\_1(a\_{k-1}a\_{k-2} \cdots a\_0) = (a_{k-1} + \overline{a}\_{k-1})(a_{k-2} + \overline{a}\_{k-2}) \cdots (\overline{a}\_0 + a\_0), $$

which is just the string $11 \cdots 1$ (with $k$ ones). Hence if we simply add $1$ to $\mathrm{comp}\_1(a\_{k-1}a\_{k-2} \cdots a\_0)$, we get precisely the string we want. We call the result the **two's complement** of $a_{k-1}a_{k-2} \cdots a_0$ and denote it by $\mathrm{comp}\_2(a\_{k-1}a\_{k-2} \cdots a\_0)$.

To sum up, this shows that to find $\mathrm{enc}([-m])$, we first take the encoding of $[m]$, and then take its two's complement, i.e.

$$ \mathrm{enc}([-m]) = \mathrm{comp}\_2(\mathrm{enc}([m])). $$

The encoding of $[m]$ is easy to find, since it is almost certainly already stored in memory or in a register (since we wish to perform an operation on $[m]$, so it better be available to us!), and it is also easy to compute the two's complement of a binary string: Just flip all bits and add one. In practice, what usually happens is that all bits are flipped, and the lowest-order full adder in the ALU is given $1$ as a carry-in.

Put another way, $\mathrm{comp}\_2(a_{k-1}a_{k-2} \cdots a_0)$ is the *additive inverse* of $a_{k-1}a_{k-2} \cdots a_0$ in $\{0,1\}^k$.


## Carry and overflow

Let $[n]$ and $[m]$ be residue classes modulo $2^k$. We say that the addition $[n] + [m]$ exhibits a **carry** if

$$ \mathrm{unsigned}([n] + [m]) < \mathrm{unsigned}([n]) + \mathrm{unsigned}([m]). $$

Notice that the addition on the left-hand side takes place in $\mathbb{Z}/2^{k}\mathbb{Z}$, while the addition of the right-hand side takes place in $\mathbb{Z}$ (or even in $\mathbb{N}$). The above inequality could be replaced by '$\neq$', since the left-hand side is never greater than the right-hand side. It is easy to see that a carry happens precisely when

$$ \mathrm{unsigned}([n]) + \mathrm{unsigned}([m]) \geq 2^k. $$

In other words, when the sum of the two unsigned interpretations lies outside of the allowed range $[0,2^{k}-1]$ for unsigned values. (Recall that we *chose* to allow the unsigned interpretations of a residue class to only lie in this interval. We could just as well have *chosen* something different.)

Furthermore, we say that the addition $[n] + [m]$ exhibits **overflow** if

$$ \mathrm{signed}([n] + [m]) < \mathrm{signed}([n]) + \mathrm{signed}([m]), $$

and that it exhibits **underflow** if

$$ \mathrm{signed}([n] + [m]) > \mathrm{signed}([n]) + \mathrm{signed}([m]). $$

Overflow and underflow are often grouped together and simply termed 'overflow'. In the case of overflow, this happens just when

$$ \mathrm{signed}([n]) + \mathrm{signed}([m]) \geq 2^{k-1}, $$

i.e. when the sum of the two signed interpretations lies *above* the allowed range $[-2^{k-1},2^{k-1}-1]$ for signed values. Notice that for overflow to happen, both signed values must be *positive*. Hence both signed values must lie in the interval $[0,2^{k-1}-1]$, and so their sum (in $\mathbb{Z}$) lies in $[0,2^k-1]$. Thus in the case of overflow, a carry cannot happen. In fact, the above says that the sum actually lies in $[2^{k-1},2^k-1]$. But taking the signed interpretation of this value yields a *negative* number.

To sum up: Overflow happens when the sum of two positive signed numbers is a negative signed number. This cannot happen together with a carry.

Similarly, underflow happens just when

$$ \mathrm{signed}([n]) + \mathrm{signed}([m]) < -2^{k-1}, $$

i.e. when the sum of the two signed interpretations lies *below* the allowed range for signed values. Opposite to the case with overflow, underflow can only happen when both signed values are *negative*, and their sum will now be *positive*. Taking the unsigned interpretation, we see that in this case a carry does occur.

In total we see that overflow and carry cannot occur together, while underflow is always accompanied by a carry (though not vice-versa!).