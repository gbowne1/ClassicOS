#include "isr.h"

void isr_handler(struct isr_regs regs) {
    switch(regs.int_no) {
        case DIVIDE_BY_ZERO_INTERRUPT:
            // Handle Divide By Zero Interrupt
            break;
        case DOUBLE_FAULT_INTERRUPT:
            // Handle Double Fault Interrupt
            break;
        case PAGE_FAULT_INTERRUPT:
            // Handle Page Fault Interrupt
            break;
        case GENERAL_PROTECTION_FAULT_INTERRUPT:
            // Handle General Protection Fault Interrupt
            break;
        default:
            // Handle other interrupts or implement error handling
            break;
    }
}
