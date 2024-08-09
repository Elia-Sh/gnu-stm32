/* Linker script for STM32F103C8T6 */

MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 64K
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    .text : {
        KEEP(*(.isr_vector)) /* Interrupt vector table */
        *(.text*)            /* All .text sections */
        *(.rodata*)         /* All .rodata sections */
    } > FLASH

    .bss : {
        *(.bss*)
    } > RAM

    .data : {
        *(.data*)
    } > RAM AT > FLASH
}

