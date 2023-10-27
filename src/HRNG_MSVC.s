global ?GenerateSeed_u64@HRNG@lwe@@SA_KXZ
global ?Generate_u64@HRNG@lwe@@SA_K_K0@Z
global ?Generate_u64@HRNG@lwe@@SA_KXZ
global ?GenerateSeed_s64@HRNG@lwe@@SA_JXZ
global ?Generate_s64@HRNG@lwe@@SA_J_J0@Z
global ?Generate_s64@HRNG@lwe@@SA_JXZ
global ?GenerateSeed_u32@HRNG@lwe@@SAIXZ
global ?Generate_u32@HRNG@lwe@@SAIII@Z
global ?Generate_u32@HRNG@lwe@@SAIXZ
global ?GenerateSeed_s32@HRNG@lwe@@SAHXZ
global ?Generate_s32@HRNG@lwe@@SAHHH@Z
global ?Generate_s32@HRNG@lwe@@SAHXZ
global ?GenerateSeed_u16@HRNG@lwe@@SAIXZ
global ?Generate_u16@HRNG@lwe@@SAGGG@Z
global ?Generate_u16@HRNG@lwe@@SAGXZ
global ?GenerateSeed_s16@HRNG@lwe@@SAFXZ
global ?Generate_s16@HRNG@lwe@@SAFFF@Z
global ?Generate_s16@HRNG@lwe@@SAFXZ
global ?GenerateSeed_u8@HRNG@lwe@@SAEXZ
global ?Generate_u8@HRNG@lwe@@SAEEE@Z
global ?Generate_u8@HRNG@lwe@@SAEXZ
global ?GenerateSeed_s8@HRNG@lwe@@SACXZ
global ?Generate_s8@HRNG@lwe@@SACCC@Z
global ?Generate_s8@HRNG@lwe@@SACXZ

section .text
    ?GenerateSeed_u64@HRNG@lwe@@SA_KXZ:
        RDSEED RAX
        RET

    ?Generate_u64@HRNG@lwe@@SA_K_K0@Z:
        RDRAND RAX
        MOV R8, RDX
        SUB R8, RCX
        XOR RDX, RDX
        DIV R8
        MOV RAX, RDX
        ADD RAX, RCX
        RET

    ?Generate_u64@HRNG@lwe@@SA_KXZ:
        RDRAND RAX
        RET

    ?GenerateSeed_s64@HRNG@lwe@@SA_JXZ:
        RDSEED RAX
        RET

    ?Generate_s64@HRNG@lwe@@SA_J_J0@Z:
        RDRAND RAX
        MOV R8, RDX
        SUB R8, RCX
        XOR RDX, RDX
        DIV R8
        MOV RAX, RDX
        ADD RAX, RCX
        RET

    ?Generate_s64@HRNG@lwe@@SA_JXZ:
        RDRAND RAX
        RET

    ?GenerateSeed_u32@HRNG@lwe@@SAIXZ:
        RDSEED EAX
        RET

    ?Generate_u32@HRNG@lwe@@SAIII@Z:
        RDRAND EAX
        MOV R8D, EDX
        SUB R8D, ECX
        XOR EDX, EDX
        DIV R8D
        MOV EAX, EDX
        ADD EAX, ECX
        RET

    ?Generate_u32@HRNG@lwe@@SAIXZ:
        RDRAND EAX
        RET

    ?GenerateSeed_s32@HRNG@lwe@@SAHXZ:
        RDSEED EAX
        RET

    ?Generate_s32@HRNG@lwe@@SAHHH@Z:
        RDRAND EAX
        MOV R8D, EDX
        SUB R8D, ECX
        XOR EDX, EDX
        DIV R8D
        MOV EAX, EDX
        ADD EAX, ECX
        RET

    ?Generate_s32@HRNG@lwe@@SAHXZ:
        RDRAND EAX
        RET

    ?GenerateSeed_u16@HRNG@lwe@@SAIXZ:
        RDSEED AX
        RET

    ?Generate_u16@HRNG@lwe@@SAGGG@Z:
        RDRAND AX
        MOV R8W, DX
        SUB R8W, CX
        XOR DX, DX
        DIV R8W
        MOV AX, DX
        ADD AX, CX
        RET

    ?Generate_u16@HRNG@lwe@@SAGXZ:
        RDRAND AX
        RET

    ?GenerateSeed_s16@HRNG@lwe@@SAFXZ:
        RDSEED AX
        RET

    ?Generate_s16@HRNG@lwe@@SAFFF@Z:
        RDRAND AX
        MOV R8W, DX
        SUB R8W, CX
        XOR DX, DX
        DIV R8W
        MOV AX, DX
        ADD AX, CX
        RET

    ?Generate_s16@HRNG@lwe@@SAFXZ:
        RDRAND AX
        RET

    ?GenerateSeed_u8@HRNG@lwe@@SAEXZ:
        RDSEED AX
        RET

    ?Generate_u8@HRNG@lwe@@SAEEE@Z:
        RDRAND AX
        MOV R8W, DX
        SUB R8W, CX
        XOR DX, DX
        DIV R8W
        MOV AX, DX
        ADD AX, CX
        RET

    ?Generate_u8@HRNG@lwe@@SAEXZ:
        RDRAND AX
        RET

    ?GenerateSeed_s8@HRNG@lwe@@SACXZ:
        RDSEED AX
        RET

    ?Generate_s8@HRNG@lwe@@SACCC@Z:
        RDRAND AX
        MOV R8W, DX
        SUB R8W, CX
        XOR DX, DX
        DIV R8W
        MOV AX, DX
        ADD AX, CX
        RET

    ?Generate_s8@HRNG@lwe@@SACXZ:
        RDRAND AX
        RET