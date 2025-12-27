#ifndef TYPES_H
#define TYPES_H

// ----------------------------
// Fixed-width integer types
// ----------------------------
typedef unsigned char      uint8_t;
typedef signed char        int8_t;
typedef unsigned short     uint16_t;
typedef signed short       int16_t;
typedef unsigned int       uint32_t;
typedef signed int         int32_t;
typedef unsigned long long uint64_t;
typedef signed long long   int64_t;

// ----------------------------
// Boolean & NULL definitions
// ----------------------------
#ifndef __cplusplus
typedef enum { false = 0, true = 1 } bool;
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

// ----------------------------
// OS subsystem types
// ----------------------------
typedef int32_t  ssize_t;

typedef uint32_t phys_addr_t; // Physical address
typedef uint32_t virt_addr_t; // Virtual address

typedef uint32_t pid_t;       // Process ID
typedef uint32_t tid_t;       // Thread ID

// ----------------------------
// Bitfield & utility macros
// ----------------------------
#define BIT(n) (1U << (n))
#define BITS(m, n) (((1U << ((n) - (m) + 1)) - 1) << (m))

// Align value to next multiple of alignment
#define ALIGN_UP(val, align) (((val) + ((align)-1)) & ~((align)-1))
#define ALIGN_DOWN(val, align) ((val) & ~((align)-1))

// ----------------------------
// Attributes for structures
// ----------------------------
#define PACKED      __attribute__((packed))
#define ALIGN(x)    __attribute__((aligned(x)))

// ----------------------------
// Likely/unlikely branch hints
// (for future optimization use)
// ----------------------------
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#endif // TYPES_H
