global _ZN3CPU6RDTSCPEPNS_3TSCE
global _ZN3CPU15GetManufacturerEPc
global _ZN3CPU11GetInfoBitsEv
global _ZN3CPU16GetFeatureBits_1Ev
global _ZN3CPU16GetFeatureBits_2Ev
global _ZN3CPU19GetExtFeatureBits_1Ev
global _ZN3CPU19GetExtFeatureBits_2Ev
global _ZN3CPU19GetExtFeatureBits_3Ev
global _ZN3CPU8GetBrandEPc

section .text
    _ZN3CPU6RDTSCPEPNS_3TSCE:
        RDTSCP
        MOV DWORD [RDI], ECX
        MOV DWORD [RDI + 4], EDX
        MOV DWORD [RDI + 8], EAX
        RET

    _ZN3CPU15GetManufacturerEPc:
        PUSH RBX

        XOR EAX, EAX
        CPUID

        MOV DWORD [RDI], EBX
        MOV DWORD [RDI + 4], EDX
        MOV DWORD [RDI + 8], ECX

        POP RBX

        RET

    _ZN3CPU11GetInfoBitsEv:
        PUSH RBX

        MOV EAX, 1
        CPUID

        POP RBX

        RET

    _ZN3CPU16GetFeatureBits_1Ev:
        PUSH RBX

        MOV EAX, 1
        CPUID

        MOV EAX, EDX

        POP RBX

        RET

    _ZN3CPU16GetFeatureBits_2Ev:
        PUSH RBX

        MOV EAX, 1
        CPUID

        MOV EAX, ECX

        POP RBX

        RET

    _ZN3CPU19GetExtFeatureBits_1Ev:
        PUSH RBX

        MOV EAX, 7
        XOR ECX, ECX
        CPUID

        MOV EAX, EBX

        POP RBX

        RET

    _ZN3CPU19GetExtFeatureBits_2Ev:
        PUSH RBX

        MOV EAX, 7
        XOR ECX, ECX
        CPUID

        MOV EAX, ECX

        POP RBX

        RET

    _ZN3CPU19GetExtFeatureBits_3Ev:
        PUSH RBX

        MOV EAX, 7
        XOR ECX, ECX
        CPUID

        MOV EAX, EDX

        POP RBX

        RET

    _ZN3CPU8GetBrandEPc:
        PUSH RBX

        MOV EAX, 80000002h
        CPUID
        MOV DWORD [RDI], EAX
        MOV DWORD [RDI + 4], EBX
        MOV DWORD [RDI + 8], ECX
        MOV DWORD [RDI + 12], EDX

        MOV EAX, 80000003h
        CPUID
        MOV DWORD [RDI + 16], EAX
        MOV DWORD [RDI + 20], EBX
        MOV DWORD [RDI + 24], ECX
        MOV DWORD [RDI + 28], EDX

        MOV EAX, 80000004h
        CPUID
        MOV DWORD [RDI + 32], EAX
        MOV DWORD [RDI + 36], EBX
        MOV DWORD [RDI + 40], ECX
        MOV DWORD [RDI + 44], EDX

        POP RBX

        RET