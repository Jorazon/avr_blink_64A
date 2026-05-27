#ifndef PRESCALE_H
#define PRESCALE_H 1
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000ul /* 16 MHz external crystal */
#endif

/* 
 * Prescale dividers.
 * For Clock Select Bit descriptions refer to Tables
 * 20-6 for Timer/Counter 1
 * 21-6 for Timer/Counter 0
 * 22-6 for Timer/Counter 2
 */
const uint16_t prescale_divider[8] = {0,1,8,64,256,1024,0,0};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PRESCALE_H */
