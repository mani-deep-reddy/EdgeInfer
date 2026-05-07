.syntax unified
.cpu cortex-m3
.thumb

/* Vector table */
.section .vector_table, "a"
.word _estack           /* Initial stack pointer */
.word Reset_Handler     /* Reset handler */

/* Reset handler */
.section .text
.type Reset_Handler, %function
Reset_Handler:
    /* Set up stack pointer (already done by vector table) */
    bl main
    /* Halt if main returns */
    b .

.size Reset_Handler, .-Reset_Handler
