# Ze chatbot - 
`https://duckduckgo.com/?q=DuckDuckGo+AI+Chat&ia=chat&duckai=1`

# Prompt - 
```
resume from previous session, 
you are a low level, embedded developer, 

developing for "Arm Cortex-M3"
on a fake clone of development board nicknamed bluepill and model of: "STM32F103C8T6"

your limitation is not to use any IDE, 
only "arm-none-eabi-gcc"

your goal is to write a low level c code to be compiled to elf file and be flashed to the board,
the code is the simplest "echo" via serial uart port
```

the outputs - 

To create a simple echo program for the STM32F103C8T6 (Blue Pill) using the ARM Cortex-M3 architecture, we will write low-level C code that initializes the UART for serial communication and echoes back any received characters. Below are the steps and the code to achieve this.
Step 1: Set Up the Environment

Make sure you have the `arm-none-eabi-gcc` toolchain installed on your system. You can compile the code using the command line.
Step 2: Create the C Code

Create a file named main.c and add the following code:
```
#include <stdint.h>

#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x40010800
#define USART1_BASE     0x40013800

#define RCC_APB2ENR    *(volatile uint32_t *)(RCC_BASE + 0x18)
#define GPIOA_CRL      *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define USART1_CR1     *(volatile uint32_t *)(USART1_BASE + 0x00)
#define USART1_BRR     *(volatile uint32_t *)(USART1_BASE + 0x08)
#define USART1_SR      *(volatile uint32_t *)(USART1_BASE + 0x00)
#define USART1_DR      *(volatile uint32_t *)(USART1_BASE + 0x04)

#define USART_SR_TXE   (1 << 7)  // Transmit data register empty
#define USART_SR_RXNE  (1 << 5)  // Read data register not empty

void delay(volatile uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}

void uart_init(void) {
    // Enable GPIOA and USART1 clocks
    RCC_APB2ENR |= (1 << 2);  // Enable GPIOA clock
    RCC_APB2ENR |= (1 << 14); // Enable USART1 clock

    // Set PA9 (TX) and PA10 (RX) to alternate function push-pull
    GPIOA_CRL &= ~(0xFF);      // Clear PA9 and PA10 configuration
    GPIOA_CRL |= 0x0B00;       // PA9: Alternate function push-pull, max speed 2 MHz
    GPIOA_CRL |= 0x00B0;       // PA10: Input floating

    // Configure USART1
    USART1_BRR = 0x1D4C;       // Set baud rate to 9600 (assuming 72 MHz clock)
    USART1_CR1 |= (1 << 13);   // Enable USART
    USART1_CR1 |= (1 << 3);    // Enable transmitter
    USART1_CR1 |= (1 << 2);    // Enable receiver
}

void uart_send(char c) {
    while (!(USART1_SR & USART_SR_TXE)); // Wait until TXE is set
    USART1_DR = c;                       // Send character
}

char uart_receive(void) {
    while (!(USART1_SR & USART_SR_RXNE)); // Wait until RXNE is set
    return USART1_DR;                     // Read character
}

int main(void) {
    uart_init();

    while (1) {
        char c = uart_receive(); // Receive character
        uart_send(c);           // Echo character back
    }

    return 0;
}
```

Step 3: Create a Linker Script

Create a file named linker_script.ld with the following content:
```
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
```
Step 4: Compile the Code

Use the following command to compile the code and create an ELF file:
```
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -T linker_script.ld -o main.elf main.c -nostartfiles
```

Step 5: Flash the Code to the Board

You can use a tool like st-flash or openocd to flash the compiled ELF file to the Blue Pill board. For example, using st-flash:
```
st-flash write main.elf 0x8000000
```


Step 6: Test the Echo Functionality

Connect the Blue Pill board to your computer via a USB-to-Serial adapter, open a terminal program (like PuTTY or minicom), and set the baud rate to 9600. You should be able to type characters, and they will be echoed back.
Conclusion

This code initializes the UART on the STM32F103C8T6 and implements a simple echo functionality. You can modify the code further to add more features as needed.


# To Debug
run st-util which will open `gdb-server`
```
$ st-util
st-util 1.8.0
2024-08-09T21:23:46 INFO common.c: NRST is not connected --> using software reset via AIRCR
2024-08-09T21:23:46 INFO common.c: F1xx_HD: 64 KiB SRAM, 256 KiB flash in at least 2 KiB pages.
2024-08-09T21:23:46 INFO gdb-server.c: Listening at *:4242...
```

open gdb client -
```
arm-none-eabi-gdb main.elf
```
within gdb connect to gdb server and enjoy debugging
```
(gdb) target remote :4242
(gdb) load
(gdb) monitor reset init
(gdb) continue
```
Examine uart
```
(gdb) break uart_send
(gdb) break uart_receive
(gdb) monitor reset init
(gdb) continue
```


# Note `main.c` is re-written "counter" print into serial
It's a useful LLM trick, re-write to perform another action,
in addition to prompt-definition "you are a <...> and your goal is to <...>"
