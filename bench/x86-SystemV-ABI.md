# x86-64

The stack grows downwards.

Parameters to functions are passed in the registers `rdi, rsi, rdx, rcx, r8, r9`, and further values are passed on the stack in reverse order.

Parameters passed on the stack may be modified by the called function. Functions are called using the call instruction that pushes the address of the next instruction to the stack and jumps to the operand.

Functions return to the caller using the ret instruction that pops a value from the stack and jump to it. The stack is 16-byte aligned just before the call instruction is called.

* Functions preserve the registers rbx, rsp, rbp, r12, r13, r14, and r15
* rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11 are scratch registers.
* The return value is stored in the rax register, or if it is a 128-bit value, then the higher 64-bits go in rdx.
* Optionally, functions push rbp such that the caller-return-rip is 8 bytes above it, and set rbp to the address of the saved rbp. This allows iterating through the existing stack frames. This can be eliminated by specifying the -fomit-frame-pointer GCC option.

### Signal handlers
Signal handlers are executed on the same stack, but 128 bytes known as the red zone is subtracted from the stack before anything is pushed to the stack. This allows small leaf functions to use 128 bytes of stack space without reserving stack space by subtracting from the stack pointer. The red zone is well-known to cause problems for x86-64 kernel developers, as the CPU itself doesn't respect the red zone when calling interrupt handlers. This leads to a subtle kernel breakage as the ABI contradicts the CPU behavior. The solution is to build all kernel code with -mno-red-zone or by handling interrupts in kernel mode on another stack than the current (and thus implementing the ABI).
