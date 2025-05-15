#include "syscalls.h"
#include "scheduler.h"
#include <stdarg.h>

void syscall_handler(int code, va_list args) {
    switch (code) {
        case SYSCALL_INIT:
            scheduler_init();
            break;
        case SYSCALL_SPAWN: {
            void (*entry)(void) = va_arg(args, void (*)(void));
            scheduler_add_task(entry);
            break;
        }
        case SYSCALL_YIELD:
            scheduler_yield();
            break;
        default:
            // Unknown syscall
            break;
    }
}

void syscall(int code, ...)
{
    va_list args;
    va_start(args, code);
    syscall_handler(code, args);
    va_end(args);
}
