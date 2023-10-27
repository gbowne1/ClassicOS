global _ZN3lwe4HRNG16GenerateSeed_u64Ev
global _ZN3lwe4HRNG12Generate_u64Emm
global _ZN3lwe4HRNG12Generate_u64Ev
global _ZN3lwe4HRNG16GenerateSeed_s64Ev
global _ZN3lwe4HRNG12Generate_s64Ell
global _ZN3lwe4HRNG12Generate_s64Ev
global _ZN3lwe4HRNG16GenerateSeed_u32Ev
global _ZN3lwe4HRNG12Generate_u32Ejj
global _ZN3lwe4HRNG12Generate_u32Ev
global _ZN3lwe4HRNG16GenerateSeed_s32Ev
global _ZN3lwe4HRNG12Generate_s32Eii
global _ZN3lwe4HRNG12Generate_s32Ev
global _ZN3lwe4HRNG16GenerateSeed_u16Ev
global _ZN3lwe4HRNG12Generate_u16Ett
global _ZN3lwe4HRNG12Generate_u16Ev
global _ZN3lwe4HRNG16GenerateSeed_s16Ev
global _ZN3lwe4HRNG12Generate_s16Ess
global _ZN3lwe4HRNG12Generate_s16Ev
global _ZN3lwe4HRNG15GenerateSeed_u8Ev
global _ZN3lwe4HRNG11Generate_u8Ehh
global _ZN3lwe4HRNG11Generate_u8Ev
global _ZN3lwe4HRNG15GenerateSeed_s8Ev
global _ZN3lwe4HRNG11Generate_s8Eaa
global _ZN3lwe4HRNG11Generate_s8Ev

section .text
    _ZN3lwe4HRNG16GenerateSeed_u64Ev:
        RDSEED RAX
        RET

    _ZN3lwe4HRNG12Generate_u64Emm:
        RDRAND RAX
        SUB RDI, RCX
        XOR RDX, RDX
        DIV RDI
        MOV RAX, RDX
        ADD RAX, RCX
        RET

    _ZN3lwe4HRNG12Generate_u64Ev:
        RDRAND RAX
        RET

    _ZN3lwe4HRNG16GenerateSeed_s64Ev:
        RDSEED RAX
        RET

    _ZN3lwe4HRNG12Generate_s64Ell:
        RDRAND RAX
        SUB RDI, RCX
        XOR RDX, RDX
        DIV RDI
        MOV RAX, RDX
        ADD RAX, RCX
        RET

    _ZN3lwe4HRNG12Generate_s64Ev:
        RDRAND RAX
        RET

    _ZN3lwe4HRNG16GenerateSeed_u32Ev:
        RDSEED EAX
        RET

    _ZN3lwe4HRNG12Generate_u32Ejj:
        RDRAND EAX
        SUB EDI, ECX
        XOR EDX, EDX
        DIV EDI
        MOV EAX, EDX
        ADD EAX, ECX
        RET

    _ZN3lwe4HRNG12Generate_u32Ev:
        RDRAND EAX
        RET

    _ZN3lwe4HRNG16GenerateSeed_s32Ev:
        RDSEED EAX
        RET

    _ZN3lwe4HRNG12Generate_s32Eii:
        RDRAND EAX
        SUB EDI, ECX
        XOR EDX, EDX
        DIV EDI
        MOV EAX, EDX
        ADD EAX, ECX
        RET

    _ZN3lwe4HRNG12Generate_s32Ev:
        RDRAND EAX
        RET

    _ZN3lwe4HRNG16GenerateSeed_u16Ev:
        RDSEED AX
        RET

    _ZN3lwe4HRNG12Generate_u16Ett:
        RDRAND AX
        SUB DI, CX
        XOR DX, DX
        DIV DI
        MOV AX, DX
        ADD AX, CX
        RET

    _ZN3lwe4HRNG12Generate_u16Ev:
        RDRAND AX
        RET

    _ZN3lwe4HRNG16GenerateSeed_s16Ev:
        RDSEED AX
        RET

    _ZN3lwe4HRNG12Generate_s16Ess:
        RDRAND AX
        SUB DI, CX
        XOR DX, DX
        DIV DI
        MOV AX, DX
        ADD AX, CX
        RET

    _ZN3lwe4HRNG12Generate_s16Ev:
        RDRAND AX
        RET

    _ZN3lwe4HRNG15GenerateSeed_u8Ev:
        RDSEED AX
        RET

    _ZN3lwe4HRNG11Generate_u8Ehh:
        RDRAND AX
        SUB DI, CX
        XOR DX, DX
        DIV DI
        MOV AX, DX
        ADD AX, CX
        RET

    _ZN3lwe4HRNG11Generate_u8Ev:
        RDRAND AX
        RET

    _ZN3lwe4HRNG15GenerateSeed_s8Ev:
        RDSEED AX
        RET

    _ZN3lwe4HRNG11Generate_s8Eaa:
        RDRAND AX
        SUB DI, CX
        XOR DX, DX
        DIV DI
        MOV AX, DX
        ADD AX, CX
        RET

    _ZN3lwe4HRNG11Generate_s8Ev:
        RDRAND AX
        RET