# Assembly

## ARM32 assembly

### Simple function calls

Strictly speaking there is no such thing as functions in ARM (just as there are not loops, arrays, etc.). The usual way to implement function-like constructs in ARM, and in assembly in general, is simply to place a label, say `function`, at the first instruction in the body of the function. To call the function we jump to this label.

We immediately run into several issues: First of all, how do we know where to return to? Before we call the function (i.e. jump to the function label) we must save the address of where the function will return to, i.e. the address just beyond the one stored by the program counter `pc`. We do this in the `r14` register, which is known as the 'link register' and thus has the alias `lr`. That is, before we call the function we save `pc` in `lr`:

    mov lr, pc
    b function
    <to be executed after returning>

Notice that this only works because `pc` (for historical reasons) turns out to contain the address of the instruction *after* the next one to be executed. The next instruction is `b function`, and the one after that is the one we wish to execute once the function returns.

We obviously don't want to remember this fact about `pc`. Instead we use the `bl` ('branch and link') instruction, which saves the correct return address in `lr` and branches to a label:

    bl function

To return to the caller, we simply copy the content of `lr` back into `pc`:

    mov pc, lr

(Notice the symmetry between calling a function and returning from a function.) Again there is a simpler method provided by the `bx` ('branch and exchange') instruction. The `x` ('exchange') has to do with switching between ARM and Thumb instructions, but this is not relevant to us here. The `bx` instruction, as opposed to e.g. `b`, lets us (unconditionally) branch to an address contained in a register instead of a label:

    bx lr

In total, we call a function with the instruction `bl function`, and return with the instruction `bx lr`.


### Calling conventions

Recall that by the calling conventions for ARM32, the callee must save the contents of some of the registers, namely `r4`-`r11` (except maybe `r9`). If the callee wishes to use these registers, their values must somehow be preserved and restored before returning. This usually cannot be done by using the other registers (`r0`-`r3`) but must be done by register spilling (i.e. saving them to memory). The usual way of doing this is to push the registers to the call stack at the beginning of the function call, and then pop them before returning.

ARM (in ARM mode as opposed to in Thumb mode) does not include push and pop instructions, though pseudo-instructions `push` and `pop` do exist. Instead we use the instructions `stmdb` and `ldmia`, which stand for '**st**ore **m**ultiple, **d**ecrement **b**efore' and '**l**oa**d** **m**ultiple, **i**ncrement **a**fter'. In fact, I personally prefer the pseudo-instructions `stmfd` and `ldmfd`, where `fd` in both cases stands for 'full descending', referring to the fact that ARM's call stack is a full descending stack (i.e. it grows downwards, and the stack pointer points to the lowest element as opposed to the address just beyond the lowest element). We thus do something like:

    stmfd sp!, {r4-r11}
    <function body>
    ldmfd sp!, {r4-r11}
    bx lr

The syntax `stmfd sp!, {r4-r11}` means the following: The register `sp` is the base register containing the address at which the contents of the registers should be stored. The exclamation mark `!` means that the value of `sp` will be overwritten by the instruction. Finally, `{r4-r11}` denotes the registers to be saved. The registers are stored in sequence such that registers with lower numbers are stored at lower memory addresses. Since the stack is descending, this means that the above instruction is essentially equivalent to the instructions `push r11`, `push r8`, ..., `push r4`.


#### Nested function calls

The above is sufficient if functions never call other functions. But if a function `f1` calls another function `f2`, then the above does not quite work: For when `f1` calls `f2` it saves the return address in `lr`, overwriting the location to which it should jump when it itself returns. Thus we need to save `lr` before calling `f2`.

We can simply save `lr` on the stack. The function `f1` will then look as follows:

    stmfd sp!, {r4-r11, lr}
    <function body>
    ldmfd sp!, {r4-r11, lr}
    bx lr

Of course we omit saving any registers we do not need to save. In particular, if `f1` calls another function but does not need to save any registers, we can simply do:

    stmfd sp!, {lr}
    <function body>
    ldmfd sp!, {lr}
    bx lr


### Stack frames

Finally, we also wish to store local variables on the stack. Since the stack can thus grow and shrink during the execution of a function, we need a fixed reference point to keep track of where data is. We do this using a **frame pointer**, saved in register `fp` (`r11`). While a function is responsible for saving its own link register, it is the callee that must preserve the frame pointer of the caller. Thus TODO