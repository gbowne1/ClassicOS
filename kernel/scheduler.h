#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 8
#define STACK_SIZE 1024 // in bytes

typedef struct task {
    uint32_t id;
    
    // The most important field: 
    // Where was the stack pointer when we last left this task?
    uint32_t *stack_ptr; 
    
    struct task *next;
} task_t;

void scheduler_init();
void scheduler_add_task(void (*entry)(void));
void scheduler_schedule();
void scheduler_yield();

#endif // SCHEDULER_H
