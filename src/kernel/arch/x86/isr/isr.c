#include "isr.h"
#include <stdbool.h>
#include <stdio.h>
static bool initialized = false;

// Declaration of save_state()
void save_state(void);

// Declaration of kernel_panic_or_recover()
void kernel_panic_or_recover(void);

void isr_handler(struct isr_regs regs) {
    if (!initialized) {
        // Initialization logic
        printf("Initializing ISR...\n");

        // Clear all interrupt flags
        clear_interrupt(DIVIDE_BY_ZERO_INTERRUPT);
        clear_interrupt(DOUBLE_FAULT_INTERRUPT);
        clear_interrupt(PAGE_FAULT_INTERRUPT);
        clear_interrupt(GENERAL_PROTECTION_FAULT_INTERRUPT);

        initialized = true;
    }

    switch(regs.int_no) {
        case DIVIDE_BY_ZERO_INTERRUPT:
            // Handle Divide By Zero Interrupt
            printf("Divide by zero error occurred!\n");
            save_state();
            kernel_panic_or_recover();
            break;
        // Add cases for other interrupts...
        default:
            // Handle other interrupts or implement error handling
            break;
    }
}
