#ifndef __LIMITS__
#define __LIMITS__ 1

#ifdef __cplusplus
extern "C" {
#endif

/* avr-gcc: int is 16-bit, long is 32-bit */
#define INT_MAX   32767            /* +2^15 - 1 */
#define INT_MIN   (-32768)         /* -2^15     */

#define UINT_MAX  65535U           /* 2^16 - 1 */

/* long is 32-bit on AVR */
#define LONG_MAX  2147483647L
#define LONG_MIN  (-2147483648L)

/* long long is 64-bit */
#define LLONG_MAX  9223372036854775807LL
#define LLONG_MIN  (-9223372036854775806854775808LL)

/* char is signed by default on avr-gcc unless you use -funsigned-char */
#define CHAR_BIT  8
#define CHAR_MAX  127
#define CHAR_MIN  (-128)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIMITS__ */
