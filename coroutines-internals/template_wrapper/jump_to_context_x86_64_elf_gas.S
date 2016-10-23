
/*
  Slightly modified version of Oliver Kowalke's routine for boost::context
*/

/*
  Context-data suited for a context switch on a x86_64 machine following System V ABI
  calling conventions (Unix). Some unused fields are present for parallelism's sake with
  boost::context's layout.
*/

/****************************************************************************************
 *                                                                                      *
 *  ----------------------------------------------------------------------------------  *
 *  |    0    |    1    |    2    |    3    |    4     |    5    |    6    |    7    |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x0   |   0x4   |   0x8   |   0xc   |   0x10   |   0x14  |   0x18  |   0x1c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        R12        |         R13       |         R14        |        R15        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    8    |    9    |   10    |   11    |    12    |    13   |    14   |    15   |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x20  |   0x24  |   0x28  |  0x2c   |   0x30   |   0x34  |   0x38  |   0x3c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        RBX        |         RBP       |         RIP        |       EXIT        |  *
 *  ----------------------------------------------------------------------------------  *
 *                                                                                      *
 ****************************************************************************************/

.text
.globl jump_to_context
.type jump_to_context,@function
.align 16
jump_to_context:
    pushq  %rbp  /* save RBP */
    pushq  %rbx  /* save RBX */
    pushq  %r15  /* save R15 */
    pushq  %r14  /* save R14 */
    pushq  %r13  /* save R13 */
    pushq  %r12  /* save R12 */

    /* store RSP (pointing to context-data) in RAX */
    movq  %rsp, %rax

    /* restore RSP (pointing to context-data) from RSI */
    movq  %rsi, %rsp

    popq  %r12  /* restore R12 */
    popq  %r13  /* restore R13 */
    popq  %r14  /* restore R14 */
    popq  %r15  /* restore R15 */
    popq  %rbx  /* restore RBX */
    popq  %rbp  /* restore RBP */

    /* restore return-address (must have been put on the new stack) */
    popq  %r8

    /*
       pass the old context as second parameter (if we're headed
       towards a landing function)
    */
    movq  %rax, %rsi
    /*
       use rdi as this parameter
       movq  %rdi, %rdi
    */

    /* indirect jump to new context */
    jmp  *%r8

.size jump_to_context,.-jump_to_context

/* Mark that we don't need executable stack. */
.section .note.GNU-stack,"",%progbits
