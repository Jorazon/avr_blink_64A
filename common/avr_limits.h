#ifndef __LIMITS__
#define __LIMITS__ 1

#ifdef __cplusplus
extern "C" {
#endif

/* char is 8 bit and signed by default unless you use -funsigned-char */

#define CHAR_BIT       8
#define UCHAR_MAX      ((unsigned char)~0U)
#define CHAR_IS_SIGNED (((char)-1) < 0)

#if CHAR_IS_SIGNED
    #define CHAR_MAX   ((signed char)(UCHAR_MAX >> 1))
    #define CHAR_MIN   ((signed char)(~CHAR_MAX))
#else
    #define CHAR_MAX   UCHAR_MAX
#endif

/* int is 16-bit */

#define UINT_MAX   (~0U)
#define INT_MAX    (UINT_MAX >> 1)
#define INT_MIN    (~INT_MAX)

/* long is 32-bit */

#define ULONG_MAX  (~0UL)
#define LONG_MAX   (ULONG_MAX >> 1)
#define LONG_MIN   (~LONG_MAX)

/* long long is 64-bit */

#define ULLONG_MAX (~0ULL)
#define LLONG_MAX  (ULLONG_MAX >> 1)
#define LLONG_MIN  (~LLONG_MAX)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIMITS__ */
