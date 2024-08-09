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

void uart_send_string(const char *str) {
    while (*str) {
        uart_send(*str++);
    }
}

void uart_send_number(uint32_t num) {
    char buffer[12]; // Enough to hold 32-bit integer in string form
    int i = 0;

    // Convert number to string
    if (num == 0) {
        uart_send('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0'; // Convert digit to character
        num /= 10;
    }

    // Send the number in reverse order
    while (i > 0) {
        uart_send(buffer[--i]);
    }
}

int main(void) {
    uart_init();
    uint32_t counter = 0;

    while (1) {
        uart_send_string("Counter: ");
        uart_send_number(counter);
        uart_send('\n'); // New line

        counter++; // Increment the counter

        // Delay for approximately 1 second
        delay(7200000); // Adjust this value based on your clock speed
    }

    return 0;
}




