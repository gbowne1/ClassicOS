#ifndef SYSCALLS_H
#define SYSCALLS_H

// Syscall numbers
typedef enum {
    SYSCALL_INIT = 0,
    SYSCALL_SPAWN,
    SYSCALL_YIELD
} syscall_code_t;

// Syscall dispatcher
void syscall_handler();

// Syscall interface
void syscall(int code, ...);

#endif // SYSCALLS_H
