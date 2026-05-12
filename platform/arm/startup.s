.syntax unified
.cpu cortex-m3
.thumb

/* Vector table */
.section .vector_table, "a"
.word _estack           /* Initial stack pointer */
.word Reset_Handler     /* Reset handler */

/* Reset handler — init .data, zero .bss, then call main */
.section .text
.type Reset_Handler, %function
Reset_Handler:
    /* Copy .data from FLASH to RAM */
    ldr r0, =_sdata       /* RAM destination start */
    ldr r1, =_sload       /* FLASH source start */
    ldr r2, =_edata       /* RAM destination end */
    subs r2, r2, r0       /* byte count */
    beq .bss_init         /* skip if zero */
    movs r3, #0           /* word offset */
.data_loop:
    ldr r4, [r1, r3]      /* load word from FLASH */
    str r4, [r0, r3]      /* store word to RAM */
    adds r3, r3, #4       /* advance offset */
    cmp r3, r2            /* compare with byte count */
    blo .data_loop        /* loop if more to copy */

    /* Zero .bss section */
.bss_init:
    ldr r0, =_sbss        /* BSS start */
    ldr r1, =_ebss        /* BSS end */
    subs r1, r1, r0       /* byte count */
    beq .call_main        /* skip if zero */
    movs r2, #0           /* zero value */
    movs r3, #0           /* word offset */
.bss_loop:
    str r2, [r0, r3]      /* zero word in RAM */
    adds r3, r3, #4       /* advance offset */
    cmp r3, r1            /* compare with byte count */
    blo .bss_loop         /* loop if more to zero */

.call_main:
    bl main
    b .                   /* halt if main returns */

.size Reset_Handler, .-Reset_Handler
