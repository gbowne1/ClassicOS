#include "scheduler.h"
#include <stddef.h>

static task_t tasks[MAX_TASKS];
static uint32_t task_stacks[MAX_TASKS][STACK_SIZE / sizeof(uint32_t)];

static int task_count = 0;
static task_t *task_list = NULL;
static task_t *current_task = NULL;

void scheduler_init() {
    // Initialize task list, etc.
    task_list = NULL;
    current_task = NULL;
    task_count = 0;
}

void scheduler_add_task(void (*entry)(void)) {
    if (task_count >= MAX_TASKS || entry == NULL) return;

    task_t *new_task = &tasks[task_count];
    new_task->id = task_count;
    new_task->entry = entry;

    // Simulate a stack pointer pointing to the "top" of the stack
    new_task->stack_ptr = &task_stacks[task_count][STACK_SIZE / sizeof(uint32_t) - 1];

    new_task->next = NULL;

    // Add to task list
    if (task_list == NULL) {
        task_list = new_task;
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
    // Very basic round-robin switch
    if (current_task && current_task->next) {
        current_task = current_task->next;
    } else {
        current_task = task_list; // Loop back
    }

    // Call context switch or simulate yielding to current_task
    // In real system: context_switch_to(current_task)
    if (current_task && current_task->entry) {
        current_task->entry();  // Simulate switching by calling
    }
}

void scheduler_yield() {
    // Stub: manually call schedule for cooperative multitasking
    scheduler_schedule();
}
