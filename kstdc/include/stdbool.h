#ifndef __CLASSIC_OS_KSTDC_STDBOOL_H__
#define __CLASSIC_OS_KSTDC_STDBOOL_H__ 1

#ifndef __cplusplus

#undef bool

#ifdef _Bool

#define bool _Bool

#undef true
#define true 1
#undef false
#define false 0

#else

typedef enum bool_t
{
	false,
	true,
} bool;

#endif

#endif

#ifdef __bool_true_false_are_defined
#undef __bool_true_false_are_defined
#endif
#define __bool_true_false_are_defined 1

#endif // __CLASSIC_OS_KSTDC_STDBOOL_H__
