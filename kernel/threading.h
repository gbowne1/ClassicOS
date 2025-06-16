#ifndef THREADING_H
#define THREADING_H

#include <stdint.h>
#include <stddef.h>

// Define a basic thread structure (Thread Control Block - TCB)
typedef struct Thread {
    void (*start_routine)(void *);   // Function pointer to the thread function
    void *arg;                       // Argument to be passed to the thread function
    uint32_t *stack;                 // Pointer to the thread's stack
    uint32_t *stack_top;             // Top of the stack (where the thread will start)
    uint32_t stack_size;             // Size of the stack
    uint32_t state;                  // Thread state (running, ready, blocked)
} Thread;

// Thread states
#define THREAD_RUNNING 0
#define THREAD_READY   1
#define THREAD_BLOCKED 2

// Thread management functions
void thread_init(void);
void thread_create(Thread *thread, void (*start_routine)(void *), void *arg);
void thread_yield(void);
void thread_exit(void);

// Scheduler function
void scheduler(void);

// Synchronization functions (mutex spinlocks)
void mutex_init(void);
void mutex_lock(void);
void mutex_unlock(void);

#endif // THREADING_H
