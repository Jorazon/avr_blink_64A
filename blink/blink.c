#ifndef F_CPU
#define F_CPU 16000000ul /* 8 MHz */
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <avr/iom64a.h>

#define USER_LED_GNR PORTB4
#define USER_LED_ORA PORTB5
#define F_USER_LED 2

static const uint16_t prescale[] = {0,1,8,64,256,1024,0,0};

void init_PB5(void);
void init_TC1B(void);

int main(void) {
    sei();
    init_PB5();
    init_TC1B();
    while(1){}
    return 0;
}

void init_PB5(void)
{
    /* LED pins as output */
    DDRB |= (1<<USER_LED_GNR);
    DDRB |= (1<<USER_LED_ORA);
    /* Set LED initial state */
    PORTB &= ~(1<<USER_LED_GNR); /* OFF */
    PORTB |= (1<<USER_LED_ORA); /* ON */
}

void init_TC1B(void)
{
    /* 1. CTC mode (clear timer on compare match) */
    TCCR1B |= (1 << WGM12);
    /* Prescale 1024 */
    uint8_t clock_mask = (1<<CS12)|(1<<CS10);
    /* Timer clock = I/O clock / 1024 */
    TCCR1B |= clock_mask;
    /* Calculate and set timer 1 compare A comparison value */
    OCR1A = (F_CPU / (prescale[clock_mask] * F_USER_LED)) - 1;
    /* Clear comparison flag */
    TIFR |= (1<<OCF1A);
    /* Enable comparison interrupt */
    TIMSK |= (1<<OCIE1A);
}

/**
 * Timer1 compare A interrupt handler
 */
ISR (TIMER1_COMPA_vect)
{
    /* Toggle pins */
    PORTB ^= (1<<USER_LED_GNR);
    PORTB ^= (1<<USER_LED_ORA);
}
