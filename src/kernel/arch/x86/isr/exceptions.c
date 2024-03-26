#include "exceptions.h"

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void DivideByZero()
{
	// Add logic to handle Divide By Zero Exception
    printf("Divide By Zero Exception\n");

    // Additional Exception Handling Logic:
    // Example: Perform specific actions for Divide By Zero scenario
    // - Log the exception to a file
    FILE *logFile = fopen("error.log", "a");
    if (logFile != NULL) {
        fprintf(logFile, "Divide By Zero Exception occurred\n");
        fclose(logFile);
    }

    // - Gracefully terminate the kernel
    printf("Exiting kernel due to Divide By Zero Exception\n");
	exit(EXIT_FAILURE);
}

void DoubleFault()
{
	// printf("Double Fault Exception");
}

void PageFault()
{
	// printf("Page Fault Exception");
}

void GeneralProtectionFault()
{
	// printf("General Protection Fault Exception");
}
