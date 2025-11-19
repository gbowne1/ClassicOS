#include "scheduler.h"
#include <stddef.h>

// Defined in context_switch.s
extern void ctx_switch(uint32_t **old_sp_ptr, uint32_t *new_sp);

static task_t tasks[MAX_TASKS];

// Stack memory area. Note: x86 Stacks grow DOWN from high to low addresses.
static uint32_t task_stacks[MAX_TASKS][STACK_SIZE / sizeof(uint32_t)];

static int task_count = 0;
static task_t *task_list = NULL;
static task_t *current_task = NULL;

void scheduler_init() {
    task_list = NULL;
    current_task = NULL;
    task_count = 0;
}

void scheduler_add_task(void (*entry)(void)) {
    if (task_count >= MAX_TASKS || entry == NULL) return;

    task_t *new_task = &tasks[task_count];
    new_task->id = task_count;

    // 1. Calculate the top of the stack (High Address)
    // We point to the very end of the array.
    uint32_t *sp = &task_stacks[task_count][STACK_SIZE / sizeof(uint32_t)];

    // 2. "Forge" the stack frame to look like ctx_switch saved it.
    // We push values onto the stack by decrementing the pointer and writing.

    // --- Return Address (EIP) ---
    sp--; 
    *sp = (uint32_t)entry; // When ctx_switch does 'ret', it pops this and jumps to 'entry'

    // --- EFLAGS ---
    sp--;
    *sp = 0x00000202; // Reserved bit set, Interrupts Enabled (IF=1). Important!

    // --- General Purpose Registers (PUSHA/POPA layout) ---
    // Order: EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    // We initialize them to 0 or meaningful values.
    sp--; *sp = 0; // EAX
    sp--; *sp = 0; // ECX
    sp--; *sp = 0; // EDX
    sp--; *sp = 0; // EBX
    sp--; *sp = 0; // ESP (Ignored by POPA)
    sp--; *sp = 0; // EBP
    sp--; *sp = 0; // ESI
    sp--; *sp = 0; // EDI

    // Save this final stack location to the TCB
    new_task->stack_ptr = sp;
    new_task->next = NULL;

    // 3. Add to linked list
    if (task_list == NULL) {
        task_list = new_task;
        current_task = new_task; // Make sure we have a current task to start
    } else {
        task_t *tail = task_list;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = new_task;
    }

    task_count++;
}

void scheduler_schedule() {
    if (!current_task) return;

    task_t *prev = current_task;
    
    // Round-robin logic
    if (current_task->next) {
        current_task = current_task->next;
    } else {
        current_task = task_list;
    }

    // Perform the ACTUAL context switch
    // We pass the address of the previous task's stack pointer storage
    // and the value of the new task's stack pointer.
    if (prev != current_task) {
        ctx_switch(&prev->stack_ptr, current_task->stack_ptr);
    }
}

void scheduler_yield() {
    scheduler_schedule();
}
