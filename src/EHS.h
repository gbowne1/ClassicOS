#if defined(_M_AMD64) || defined(_M_X64) || defined(__x86_64__)
    #define LITTLE_ENDIAN
    #define ARCH_X64
#elif defined(__i386__)
	#define LITTLE_ENDIAN
	#define ARCH_X86
#elif defined(_M_ARM64) || defined(__aarch64__)
    #define LITTLE_ENDIAN
    #define ARCH_ARM64
#else
    #error Unsupported architecture.
#endif

typedef unsigned char UInt_8;
typedef signed char SInt_8;
typedef char Int_8;
typedef unsigned short UInt_16;
typedef signed short SInt_16;
typedef short Int_16;
typedef unsigned int UInt_32;
typedef signed int SInt_32;
typedef int Int_32;

typedef UInt_8 Byte;
typedef Int_8 Char_8;
typedef Int_16 Char_16;
typedef Int_32 Char_32;

#if defined(__GNUC__)
	typedef unsigned long long UInt_64;
	typedef signed long long SInt_64;
	typedef long long Int_64;
#elif defined(_WIN32)
	typedef unsigned long UInt_64;
	typedef signed long SInt_64;
	typedef long Int_64;
#endif

#if defined(ARCH_X64)
	typedef UInt_64 USize;
	typedef SInt_64 SSize;
	typedef Int_64 Size;
#elif defined(ARCH_X86)
	typedef UInt_32 USize;
	typedef SInt_32 SSize;
	typedef Int_32 Size;
#endif

#define MAX_PATH 0x104
#define UINT_8_MAX 0xFF
#define SINT_8_MAX 0x7F
#define SINT_8_MIN 0x80
#define UINT_16_MAX 0xFFFF
#define SINT_16_MAX 0x7FFF
#define SINT_16_MIN 0x8000
#define UINT_24_MAX 0xFFFFFF
#define SINT_24_MAX 0x7FFFFF
#define SINT_24_MIN 0x800000
#define UINT_32_MAX 0xFFFFFFFF
#define SINT_32_MAX 0x7FFFFFFF
#define SINT_32_MIN 0x80000000
#define UINT_64_MAX 0xFFFFFFFFFFFFFFFF
#define SINT_64_MAX 0x7FFFFFFFFFFFFFFF
#define SINT_64_MIN 0x8000000000000000
#define FLOAT_MAX 3.40282e+038f
#define FLOAT_MIN 1.17549e-038f
#define DOUBLE_MAX 1.79769e+308
#define DOUBLE_MIN 2.22507e-308
#define LDOUBLE_MAX 1.79769e+308
#define LDOUBLE_MIN 2.22507e-308

#define INFINITE UINT_32_MAX