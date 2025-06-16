#include "threading.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_THREADS 16           // Maximum number of threads
#define THREAD_STACK_SIZE 8192   // Stack size for each thread

// The thread table stores information about all threads
static Thread thread_table[MAX_THREADS];
static uint32_t current_thread = 0;  // Index of the currently running thread
static uint32_t num_threads = 0;     // Number of active threads

// A simple mutex spinlock
static volatile int mutex_locked = 0;

// Function declaration for context_switch
void context_switch(Thread *next);

// Initialize the threading system
void thread_init(void) {
    memset(thread_table, 0, sizeof(thread_table));
    num_threads = 0;
}

// Create a new thread
void thread_create(Thread *thread __attribute__((unused)), void (*start_routine)(void *), void *arg) {
    if (num_threads >= MAX_THREADS) {
        printf("Error: Maximum thread count reached.\n");
        return;
    }

    // Find an empty slot for the new thread
    int index = num_threads++;
    thread_table[index] = (Thread){0};
    
    // Set up the new thread
    thread_table[index].start_routine = start_routine;
    thread_table[index].arg = arg;
    thread_table[index].stack_size = THREAD_STACK_SIZE;
    thread_table[index].stack = (uint32_t*)malloc(THREAD_STACK_SIZE);
    thread_table[index].stack_top = thread_table[index].stack + THREAD_STACK_SIZE / sizeof(uint32_t);

    // Initialize the stack (simulate pushing the function's return address)
    uint32_t *stack_top = thread_table[index].stack_top;
    *(--stack_top) = (uint32_t)start_routine;  // Return address (the thread's entry point)
    *(--stack_top) = (uint32_t)arg;            // Argument to pass to the thread

    // Set the thread's state to ready
    thread_table[index].state = THREAD_READY;

    // If this is the first thread, switch to it
    if (index == 0) {
        scheduler();
    }
}

// Yield the CPU to another thread
void thread_yield(void) {
    // Find the next thread in a round-robin manner
    uint32_t next_thread = (current_thread + 1) % num_threads;
    while (next_thread != current_thread && thread_table[next_thread].state != THREAD_READY) {
        next_thread = (next_thread + 1) % num_threads;
    }

    if (next_thread != current_thread) {
        current_thread = next_thread;
        scheduler();
    }
}

// Exit the current thread
void thread_exit(void) {
    thread_table[current_thread].state = THREAD_BLOCKED;  // Mark the thread as blocked (finished)
    free(thread_table[current_thread].stack);             // Free the thread's stack
    num_threads--;                                       // Decrease thread count

    // Yield to the next thread
    thread_yield();
}

// Scheduler: This function selects the next thread to run
void scheduler(void) {
    // Find the next ready thread
    uint32_t next_thread = (current_thread + 1) % num_threads;
    while (thread_table[next_thread].state != THREAD_READY) {
        next_thread = (next_thread + 1) % num_threads;
    }

    if (next_thread != current_thread) {
        current_thread = next_thread;
        context_switch(&thread_table[current_thread]);
    }
}

// Context switch to the next thread (assembly would go here to save/load registers)
void context_switch(Thread *next) {
    // For simplicity, context switching in this example would involve saving/restoring registers.
    // In a real system, you would need to save the CPU state (registers) and restore the next thread's state.
    printf("Switching to thread...\n");
    next->start_routine(next->arg);  // Start running the next thread
}

// Simple mutex functions (spinlock)
void mutex_init(void) {
    mutex_locked = 0;
}

void mutex_lock(void) {
    while (__sync_lock_test_and_set(&mutex_locked, 1)) {
        // Busy wait (spinlock)
    }
}

void mutex_unlock(void) {
    __sync_lock_release(&mutex_locked);
}
