#ifndef CLASSICOS_KLIBC_STDARG_H
#define CLASSICOS_KLIBC_STDARG_H

typedef __builtin_va_list va_list;

#ifndef va_start
#define va_start(ap, param) __builtin_va_start(ap, param)
#endif

#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_copy(dest, src) __builtin_va_copy(dest, src)

#endif  // CLASSICOS_KLIBC_STDARG_H
