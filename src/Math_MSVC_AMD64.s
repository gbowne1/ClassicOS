global ?Sqrt_AVX@Math@lwe@@CAMM@Z
global ?Sqrt_AVX@Math@lwe@@CANN@Z
global ?Sqrt_SSE@Math@lwe@@CAMM@Z
global ?Sqrt_SSE2@Math@lwe@@CANN@Z
global ?Near@Math@lwe@@SAMM@Z
global ?Near@Math@lwe@@SANN@Z
global ?Floor@Math@lwe@@SAMM@Z
global ?Floor@Math@lwe@@SANN@Z
global ?Ceil@Math@lwe@@SAMM@Z
global ?Ceil@Math@lwe@@SANN@Z
global ?Trunc@Math@lwe@@SAMM@Z
global ?Trunc@Math@lwe@@SANN@Z

section .text
    ?Sqrt_AVX@Math@lwe@@CAMM@Z:
        VSQRTPS XMM0, XMM0
        RET

    ?Sqrt_AVX@Math@lwe@@CANN@Z:
        VSQRTPD XMM0, XMM0
        RET

    ?Sqrt_SSE@Math@lwe@@CAMM@Z:
        SQRTPS XMM0, XMM0
        RET

    ?Sqrt_SSE2@Math@lwe@@CANN@Z:
        SQRTPD XMM0, XMM0
        RET

    ?Near@Math@lwe@@SAMM@Z:
        ROUNDPS XMM0, XMM0, 0
        RET

    ?Near@Math@lwe@@SANN@Z:
        ROUNDPD XMM0, XMM0, 0
        RET

    ?Floor@Math@lwe@@SAMM@Z:
        ROUNDPS XMM0, XMM0, 1
        RET

    ?Floor@Math@lwe@@SANN@Z:
        ROUNDPD XMM0, XMM0, 1
        RET

    ?Ceil@Math@lwe@@SAMM@Z:
        ROUNDPS XMM0, XMM0, 2
        RET

    ?Ceil@Math@lwe@@SANN@Z:
        ROUNDPD XMM0, XMM0, 2
        RET

    ?Trunc@Math@lwe@@SAMM@Z:
        ROUNDPS XMM0, XMM0, 3
        RET

    ?Trunc@Math@lwe@@SANN@Z:
        ROUNDPD XMM0, XMM0, 3
        RET