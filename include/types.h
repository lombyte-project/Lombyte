#ifndef LOMBYTE_TYPES_H
#define LOMBYTE_TYPES_H

/* Project-owned EE scalar types.  Keep these independent of host ABI widths. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;

typedef float f32;
typedef double f64;
typedef s32 b32;

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#define ARRAY_COUNT(array)  ((s32)(sizeof(array) / sizeof((array)[0])))
#define ARRAY_COUNTU(array) ((u32)(sizeof(array) / sizeof((array)[0])))

#endif /* LOMBYTE_TYPES_H */
