#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
//#include <avr/iom64a.h> // for reference

#include "timer_counter.h"

/**
 * Set baud rate
 */
void USART1_Init(unsigned long baud) {
    // Calculate baud rate prescale
    uint16_t ubrr = (F_CPU + 8ul * baud) / (16ul * baud) - 1ul;
    
    // Write to 12 bit Baud Rate Register
    UBRR1L = (uint8_t)(ubrr & 0xFFu);
    UBRR1H = (uint8_t)((ubrr >> 8) & 0x0Fu);
    
    // Set parity mode
    //UCSR1C &= ~((1 << UPM10) | (1 << UPM11)); // parity disabled
    
    // Set stop bits
    //UCSR1C &= ~(1 << USBS1); // 1 stop bit
    
    // 8 data bits
    UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
    //UCSR1C &= ~(1 << UCSZ12);
    
    // Asynchronous Operation
    //UCSR1C &= ~(1 << UMSEL1);
    
    // Enable Rx & Tx
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
}

/**
 * Write a single character
 */
void USART1_Transmit(unsigned char data) {
    // wait for any other transmits to complete
    while(!(UCSR1A & (1 << UDRE1)));
    // load data into transmit register
    UDR1 = data;
}

/**
 * Read a single character
 */
unsigned char USART1_Receive(void) {
    // wait for data
    while(!(UCSR1A & (1 << RXC1)));
    // return data
    return UDR1;
}

/**
 * Discard unread data from USART I/O Data Register
 */
void USART1_Flush(void) {
    unsigned char dummy;
    while ( UCSR1A & (1 << RXC1) ) dummy = UDR1;
}

/**
 * Write C string
 */
void USART1_puts(unsigned char * s) {
    while(*s != '\0') USART1_Transmit(*s++);
    // wait for transmission to complete
    while (!UCSR1A & (1<<TXC1)){};
}

int main(void) {
    USART1_Init(115200uL);
    unsigned long int counter = 0;
    unsigned char buf[16];
    while (1) {
        snprintf(buf, sizeof(buf), "%5ld", counter++);
        if (counter > 99999ul) counter = 0;
        USART1_puts(buf);
        while(USART1_Receive() != 'E'){}; // wait for LDP-M3604RI-UR ready signal
        _delay_ms(5);
    }
    return 0;
}
