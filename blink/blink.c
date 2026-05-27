#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <avr/iom64a.h> // for reference

#include "timer_counter.h"

#define USER_LED_GNR PORTB4 // First LED Pin. Mine was green.
#define USER_LED_ORA PORTB5 // Seconf LED Pin. Mine was orange.
#define F_USER_LED 2 // LED toggle fequency (Hz)

void init_PB5(void);
void init_TC1B(void);

int main(void) {
    sei(); // Enables interrupts
    init_PB5();
    init_TC1B();
    while(1){}
    return 0;
}

/**
 * Initialize Port B for LED oputput
 */
void init_PB5(void)
{
    /* Set LED pins as outputs in the Data Direction Register of Port B */
    DDRB |= (1<<USER_LED_GNR);
    DDRB |= (1<<USER_LED_ORA);
    /* Set LED initial states in the Port B Data Register */
    PORTB &= ~(1<<USER_LED_GNR); /* OFF */
    PORTB |= (1<<USER_LED_ORA); /* ON */
}

void init_TC1B(void)
{
    /* 1. CTC mode (clear timer on compare match) */
    TCCR1B |= (1 << WGM12);
    /* Prescale should be 1024 */
    uint8_t clock_mask = (1<<CS12)|(1<<CS10);
    /* Timer clock = I/O clock / 1024 */
    TCCR1B |= clock_mask;
    /* Calculate and set Timer 1 compare A comparison value */
    OCR1A = (F_CPU / (prescale_divider[clock_mask] * F_USER_LED)) - 1;
    /* Clear comparison flag */
    TIFR |= (1<<OCF1A);
    /* Enable comparison interrupt */
    TIMSK |= (1<<OCIE1A);
}

/**
 * Timer 1 compare A interrupt handler
 */
ISR (TIMER1_COMPA_vect)
{
    /* Toggle LED pins */
    PORTB ^= (1<<USER_LED_GNR);
    PORTB ^= (1<<USER_LED_ORA);
}
