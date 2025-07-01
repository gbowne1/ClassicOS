#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdarg.h>
// Syscall numbers
typedef enum {
    SYSCALL_INIT = 0,
    SYSCALL_SPAWN,
    SYSCALL_YIELD
} syscall_code_t;

// Syscall dispatcher
void syscall_handler(int code, va_list args);
// Syscall interface
void syscall(int code, ...);

#endif // SYSCALLS_H
