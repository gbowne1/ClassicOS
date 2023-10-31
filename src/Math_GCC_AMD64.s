global _ZN3Math8Sqrt_AVXEf
global _ZN3Math8Sqrt_AVXEd
global _ZN3Math8Sqrt_SSEEf
global _ZN3Math9Sqrt_SSE2Ed
global _ZN3Math4NearEf
global _ZN3Math4NearEd
global _ZN3Math5FloorEf
global _ZN3Math5FloorEd
global _ZN3Math4CeilEf
global _ZN3Math4CeilEd
global _ZN3Math5TruncEf
global _ZN3Math5TruncEd

section .text
    _ZN3Math8Sqrt_AVXEf:
        VSQRTPS XMM0, XMM0
        RET

    _ZN3Math8Sqrt_AVXEd:
        VSQRTPD XMM0, XMM0
        RET

    _ZN3Math8Sqrt_SSEEf:
        SQRTPS XMM0, XMM0
        RET

    _ZN3Math9Sqrt_SSE2Ed:
        SQRTPD XMM0, XMM0
        RET

    _ZN3Math4NearEf:
        ROUNDPS XMM0, XMM0, 0
        RET

    _ZN3Math4NearEd:
        ROUNDPD XMM0, XMM0, 0
        RET

    _ZN3Math5FloorEf:
        ROUNDPS XMM0, XMM0, 1
        RET

    _ZN3Math5FloorEd:
        ROUNDPD XMM0, XMM0, 1
        RET

    _ZN3Math4CeilEf:
        ROUNDPS XMM0, XMM0, 2
        RET

    _ZN3Math4CeilEd:
        ROUNDPD XMM0, XMM0, 2
        RET

    _ZN3Math5TruncEf:
        ROUNDPS XMM0, XMM0, 3
        RET

    _ZN3Math5TruncEd:
        ROUNDPD XMM0, XMM0, 3
        RET