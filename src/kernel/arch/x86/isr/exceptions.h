#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stddef.h>
#include <stdint.h>

void DivideByZero();

void DoubleFault();

void PageFault(uint16_t fault_address);

void GeneralProtectionFault();

#endif
