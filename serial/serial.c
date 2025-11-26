#ifndef F_CPU
#define F_CPU 16000000ul /* 16 MHz external crystal */
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
//#include <avr/iom64a.h>

#define INT_MAX   __INT_MAX__
#define INT_MIN   (-__INT_MAX__ - 1)

/*
USART1, Rx Complete
#define USART1_RX_vect            _VECTOR(30)
#define USART1_RX_vect_num        30

USART1, Data Register Empty
#define USART1_UDRE_vect            _VECTOR(31)
#define USART1_UDRE_vect_num        31

USART1, Tx Complete
#define USART1_TX_vect            _VECTOR(32)
#define USART1_TX_vect_num        32

#define UBRR1H  _SFR_MEM8(0x98)

#define UBRR1L  _SFR_MEM8(0x99)

#define UCSR1B  _SFR_MEM8(0x9A)
#define TXB81   0
#define RXB81   1
#define UCSZ12  2
#define TXEN1   3
#define RXEN1   4
#define UDRIE1  5
#define TXCIE1  6
#define RXCIE1  7

#define UCSR1A  _SFR_MEM8(0x9B)
#define MPCM1   0
#define U2X1    1
#define UPE1    2
#define DOR1    3
#define FE1     4
#define UDRE1   5
#define TXC1    6
#define RXC1    7

#define UDR1    _SFR_MEM8(0x9C)

#define UCSR1C  _SFR_MEM8(0x9D)
#define UCPOL1  0
#define UCSZ10  1
#define UCSZ11  2
#define USBS1   3
#define UPM10   4
#define UPM11   5
#define UMSEL1  6
*/

void init_UART1(unsigned long baud) {
    // Calculate baud rate prescale
    uint16_t ubrr = (F_CPU + 8ul * baud) / (16ul * baud) - 1ul;
    // Write to 12 bit Baud Rate Register
    UBRR1L = (uint8_t)(ubrr & 0xFFu);
    UBRR1H = (uint8_t)((ubrr >> 8) & 0x0Fu);
    //// Set parity mode and stop and data bits
    //UCSR1C &= ~((1 << UPM10) | (1 << UPM11)); // parity disabled
    //UCSR1C &= ~(1 << USBS1); // 1 stop bit
    // 8 data bits
    UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
    //UCSR1C &= ~(1 << UCSZ12);
    //// Asynchronous Operation
    //UCSR1C &= ~(1 << UMSEL1);
    // Enable Rx & Tx
    UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
}

static const uint16_t prescale[] = {0,1,8,64,256,1024,0,0};

void USART_Transmit(unsigned char data) {
    // wait for transmit data buffer to be empty
    while(!(UCSR1A & (1 << UDRE1)));
    // load data into transmit register
    UDR1 = data;
}

unsigned char USART_Receive(void) {
    // wait for data
    while(!(UCSR1A & (1 << RXC1)));
    // return data
    return UDR1;
}

void USART_Flush(void) {
    unsigned char dummy;
    while ( UCSR1A & (1 << RXC1) ) dummy = UDR1;
}

void USART_puts(unsigned char * s) {
    while(*s != '\0') USART_Transmit(*s++);
    // wait for transmission to complete
    while (!UCSR1A & (1<<TXC1)){};
}

int main(void) {
    init_UART1(115200uL);
    unsigned long int counter = 0;
    unsigned char buf[16];
    while (1) {
        snprintf(buf, sizeof(buf), "%5lu", counter++);
        if (counter >= 100000ul) counter = 0;
        USART_puts(buf);
        while(USART_Receive() != 'E'){};
        _delay_ms(5);
    }
    return 0;
}
