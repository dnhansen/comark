# Assembly

## ARM32 assembly

### Branching

Simple branching is done with the `b` instruction. The syntax is

    b{cond} label

where `{cond}` is an optional **condition code**. If `{cond}` is omitted, the branch is unconditional.

Examples of condition codes include `eq` and `ne`, which branch to `label` if the `Z` flag is set or clear, respectively. See [the documentation](https://developer.arm.com/documentation/dui0068/b/ARM-Instruction-Reference/Conditional-execution) for a complete list of condition codes.

We need to specify whether an instruction should set the flags or not. Most instructions, e.g. `add` and `sub`, do not set the flag, but adding the suffix `s` makes them set the flag.

For instance, the instructions

    subs r1, r1, 1
    beq zero

thus subtract 1 from `r1` and branches to the label `zero` if the result is zero.

The instruction [`cmp`](https://developer.arm.com/documentation/dui0068/b/ARM-Instruction-Reference/ARM-general-data-processing-instructions/CMP-and-CMN) may also be useful for comparing values.


### Function calls

#### Calling conventions

Recall that by the calling conventions for ARM32, the callee must save the contents of some of the registers, namely `r4`-`r8`, `r10`, `r11` (i.e. `fp`) and `r12` (i.e. `sp`), and maybe also `r9`. Note that this means that the caller must save the register `r14` (i.e. `lr`)

#### Simple function calls

Strictly speaking there is no such thing as functions in ARM (just as there are not loops, arrays, etc.). The usual way to implement function-like constructs in ARM, and in assembly in general, is simply to place a label, say `function`, at the first instruction in the body of the function. To call the function we jump to this label.

We immediately run into several issues: First of all, how do we know where to return to? Before we call the function (i.e. jump to the function label) we must save the address of where the function will return to, i.e. the address just beyond the one stored by the program counter `pc`. We do this in the `r14` register, which is known as the 'link register' and thus has the alias `lr`. That is, before we call the function we save `pc` in `lr`:

    mov lr, pc
    b function
    <to be executed after returning>

Notice that this only works because `pc` (for historical reasons) turns out to contain the address of the instruction *after* the next one to be executed. The next instruction is `b function`, and the one after that is the one we wish to execute once the function returns.

We obviously don't want to remember this fact about `pc`. Instead we use the `bl` ('branch and link') instruction, which saves the correct return address in `lr` and branches to a label:

    bl function
    <to be executed after returning>

To return to the caller, we simply copy the content of `lr` back into `pc`:

    mov pc, lr

(Notice the symmetry between calling a function and returning from a function.) Again there is a simpler method provided by the `bx` ('branch and exchange') instruction. The `x` ('exchange') has to do with switching between ARM and Thumb instructions, but this is not relevant to us here. The `bx` instruction, as opposed to e.g. `b`, lets us (unconditionally) branch to an address contained in a register instead of a label:

    bx lr

In total, we call a function with the instruction `bl function`, and return with the instruction `bx lr`.


### Using callee-saved registers

If the callee wishes to use callee-saved registers (see [above](#calling-conventions)), their values must somehow be preserved and restored before returning. This usually cannot be done by using the other registers but must be done by register spilling (i.e. saving them to memory). The usual way of doing this is to push the registers to the call stack at the beginning of the function call, and then pop them before returning. This works first of all because the stack pointer is callee-saved, so it is preserved by function calls, and secondly because a function (again by convention) does not modify the values already on the stack when the function is called.

ARM (in ARM mode as opposed to in Thumb mode) does not include explicit push and pop instructions, though pseudo-instructions `push` and `pop` do exist. Instead we use the instructions `stmdb` and `ldmia`, which stand for '**st**ore **m**ultiple, **d**ecrement **b**efore' and '**l**oa**d** **m**ultiple, **i**ncrement **a**fter'. In fact, I personally prefer the pseudo-instructions `stmfd` and `ldmfd`, where `fd` in both cases stands for 'full descending', referring to the fact that ARM's call stack is a full descending stack (i.e. it grows downwards, and the stack pointer points to the lowest element as opposed to the address just beyond the lowest element). But any combination of `stmdb`/`stmfd`/`push` and `ldmia`/`ldmfd`/`pop` will work. We thus do something like:

    stmdb sp!, {<registers>}
    <function body>
    ldmia sp!, {<registers>}
    bx lr

Note that the syntax using `push`/`pop` is slightly different, in that the stack pointer is updated automatically:

    push {<registers>}
    <function body>
    pop {<registers>}
    bx lr

Here `<registers>` is a comma-separated list of registers. The syntax `stmdb sp!, {<registers>}` means the following: The register `sp` is the base register containing the address at which the contents of the registers should be stored. The exclamation mark `!` means that the value of `sp` will be overwritten by the instruction. Finally, `{<registers>}` denotes the registers to be saved. The registers are stored in sequence such that registers with lower numbers are stored at lower memory addresses. Since the stack is descending, this means that the above instruction is essentially equivalent to pushing the elements of `registers` in reverse order.


#### Nested function calls

The above is sufficient if functions never call other functions. But if a function `f1` calls another function `f2`, then the above does not quite work: For when `f1` calls `f2` it saves the return address in `lr`, overwriting the location to which it should jump when it itself returns. Thus we need to save `lr` before calling `f2` (indeed, recall from [above](#calling-conventions) that `lr` is caller-saved).

We can simply save `lr` on the stack. The function `f1` will then look as follows:

    stmdb sp!, {<registers>, lr}
    <function body>
    ldmia sp!, {<registers>, lr}
    bx lr

Of course we omit saving any registers we do not need to save. In particular, if `f1` calls another function but does not need to save any registers, we can simply do:

    stmdb sp!, {lr}
    <function body>
    ldmia sp!, {lr}
    bx lr


### Stack frames

Finally, we also wish to store local variables on the stack. As above we rely on the fact that functions do not modify the stack, except by pushing elements and popping those same elements. Thus by decrementing (since the stack is descending) the stack pointer `sp` we may allocate memory in which to store local variables. Notice that if we, after saving `lr` and various callee-saved registers, decrement `sp` by $4n$, this allocates space for $n$ local variables with addresses `sp`, `sp + 4`, ..., `sp + 4*n`. Appropriately incrementing `sp` before returning then frees the memory again.

This is sufficient to store and use local variables on the stack. But since the stack may grow and shrink during the execution of the function, it is fairly tricky to use. Instead we store in `r11` (i.e. `fp`) a so-called **frame pointer**, which points to somewhere at the base of the function's stack frame. It does not matter precisely where it points to, but it customarily points to the location at which the function's return address (i.e. the value of `lr`) is stored.

However, [recall](#calling-conventions) that `r11` is a callee-saved register. Thus before overwriting `fp`, the old value must be stored somewhere. It is again immaterial precisely where it is stored as long as it is restored upon returning, but it is usually stored immediately below the return address (i.e. at address `fp - 4`, following the above convention).

In total, we obtain the following picture of the stack, from the point of view of the callee:

    ...
    <caller's stack frame>
    lr                          <- fp
    <caller's fp>
    <callee-saved register>
    ...
    <callee-saved register>
    <local variable>
    ...
    <local variable>            <- sp
    <non-allocated memory>

TODO: How to access non-local variables?


### System calls

We will see system calls again later when we study operating systems in more detail. A **system call** is a function that asks the operating system for a particular service, like writing to or reading from a file, exiting a program, creating or removing directories, updating access permissions, and so on.

On the Linux manual page for the [`syscall`](https://man7.org/linux/man-pages/man2/syscall.2.html) function we can see the **calling conventions** for different kinds of architectures. The Raspberry Pi uses arm/EABI (to see this, consider the output of the `readelf -h max` command, so to perform a system call we use the instruction `swi 0x0` (`swi` for 'SoftWare Interrupt'). Note that `swi` is equivalent to the instruction `svc` (for 'SuperVisor Call') used by the Unified Assembler Language (UAL). The table also indicates which register should be used to specify the system call (which on arm/EABI is `r7`), and which registers receive return values or error values.

Looking at [a list of system calls](https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/syscalls.md#arm-32_bit_eabi), we see that the code for the `write` system call is `0x04`, and that the code for the `exit` system call is `0x01`. The table also specifies in which registers arguments to the system call should be placed. For instance, the arguments to `write` should be placed in registers `r0`, `r1` and `r2`.

Furthermore, looking at the manual page for the [`write`](https://man7.org/linux/man-pages/man2/write.2.html) system call, we see what each argument does: The first argument `fd` is a **file descriptor**. On Linux many things are modelled using files, for instance standard I/O. Hence the standard output also gets a file descriptor, and it is always given the descriptor `1`. The argument `buf` is a pointer to the string to be printed, and `count` is the size of the string.

Similarly, the `exit` system call takes exactly one argument to be placed in `r0`, namely the exit code (which is usually `0` when no error has occurred, and something else when an error has in fact occurred, the code depending on the error).