global ?GetManufacturer@CPU@lwe@@SAXPEAD@Z
global ?GetInfoBits@CPU@lwe@@SAIXZ
global ?GetFeatureBits_1@CPU@lwe@@SAIXZ
global ?GetFeatureBits_2@CPU@lwe@@SAIXZ
global ?GetExtFeatureBits_1@CPU@lwe@@SAIXZ
global ?GetExtFeatureBits_2@CPU@lwe@@SAKXZ
global ?GetExtFeatureBits_3@CPU@lwe@@SAKXZ
global ?GetBrand@CPU@lwe@@SAXPEAD@Z

section .text
    ?GetManufacturer@CPU@lwe@@SAXPEAD@Z:
        PUSH RBX

        XOR EAX, EAX
        MOV R8, RCX
        CPUID

        MOV DWORD [R8], EBX
        MOV DWORD [R8 + 4], EDX
        MOV DWORD [R8 + 8], ECX

        POP RBX

        RET

    ?GetInfoBits@CPU@lwe@@SAIXZ:
        PUSH RBX

        MOV EAX, 1
        CPUID

        POP RBX

        RET

    ?GetFeatureBits_1@CPU@lwe@@SAIXZ:
        PUSH RBX

        MOV EAX, 1
        CPUID

        MOV EAX, EDX

        POP RBX

        RET

    ?GetFeatureBits_2@CPU@lwe@@SAIXZ:
        PUSH RBX

        MOV EAX, 1
        CPUID

        MOV EAX, ECX

        POP RBX

        RET

    ?GetExtFeatureBits_1@CPU@lwe@@SAIXZ:
        PUSH RBX

        MOV EAX, 7
        XOR ECX, ECX
        CPUID

        MOV EAX, EBX

        POP RBX

        RET

    ?GetExtFeatureBits_2@CPU@lwe@@SAKXZ:
        PUSH RBX

        MOV EAX, 7
        XOR ECX, ECX
        CPUID

        MOV EAX, ECX

        POP RBX

        RET

    ?GetExtFeatureBits_3@CPU@lwe@@SAKXZ:
        PUSH RBX

        MOV EAX, 7
        XOR ECX, ECX
        CPUID

        MOV EAX, EDX

        POP RBX

        RET

    ?GetBrand@CPU@lwe@@SAXPEAD@Z:
        PUSH RBX

        MOV R8, RCX

        MOV EAX, 80000002h
        CPUID
        MOV DWORD [R8], EAX
        MOV DWORD [R8 + 4], EBX
        MOV DWORD [R8 + 8], ECX
        MOV DWORD [R8 + 12], EDX

        MOV EAX, 80000003h
        CPUID
        MOV DWORD [R8 + 16], EAX
        MOV DWORD [R8 + 20], EBX
        MOV DWORD [R8 + 24], ECX
        MOV DWORD [R8 + 28], EDX

        MOV EAX, 80000004h
        CPUID
        MOV DWORD [R8 + 32], EAX
        MOV DWORD [R8 + 36], EBX
        MOV DWORD [R8 + 40], ECX
        MOV DWORD [R8 + 44], EDX

        POP RBX

        RET