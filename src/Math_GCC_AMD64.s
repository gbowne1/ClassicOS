global _ZN3lwe4Math8Sqrt_AVXEf
global _ZN3lwe4Math8Sqrt_AVXEd
global _ZN3lwe4Math8Sqrt_SSEEf
global _ZN3lwe4Math9Sqrt_SSE2Ed
global _ZN3lwe4Math4NearEf
global _ZN3lwe4Math4NearEd
global _ZN3lwe4Math5FloorEf
global _ZN3lwe4Math5FloorEd
global _ZN3lwe4Math4CeilEf
global _ZN3lwe4Math4CeilEd
global _ZN3lwe4Math5TruncEf
global _ZN3lwe4Math5TruncEd

section .text
    _ZN3lwe4Math8Sqrt_AVXEf:
        VSQRTPS XMM0, XMM0
        RET

    _ZN3lwe4Math8Sqrt_AVXEd:
        VSQRTPD XMM0, XMM0
        RET

    _ZN3lwe4Math8Sqrt_SSEEf:
        SQRTPS XMM0, XMM0
        RET

    _ZN3lwe4Math9Sqrt_SSE2Ed:
        SQRTPD XMM0, XMM0
        RET

    _ZN3lwe4Math4NearEf:
        ROUNDPS XMM0, XMM0, 0
        RET

    _ZN3lwe4Math4NearEd:
        ROUNDPD XMM0, XMM0, 0
        RET

    _ZN3lwe4Math5FloorEf:
        ROUNDPS XMM0, XMM0, 1
        RET

    _ZN3lwe4Math5FloorEd:
        ROUNDPD XMM0, XMM0, 1
        RET

    _ZN3lwe4Math4CeilEf:
        ROUNDPS XMM0, XMM0, 2
        RET

    _ZN3lwe4Math4CeilEd:
        ROUNDPD XMM0, XMM0, 2
        RET

    _ZN3lwe4Math5TruncEf:
        ROUNDPS XMM0, XMM0, 3
        RET

    _ZN3lwe4Math5TruncEd:
        ROUNDPD XMM0, XMM0, 3
        RET