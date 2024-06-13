[bits 32]

global cpuid

cpuid:
 ; Save registers
 push ebp
 mov ebp, esp
 push ebx
 push edi
 push esi

 ; Input parameter in EAX register
 mov eax, [ebp + 8] ; Assuming the input is passed on the stack

 ; Call CPUID instruction (clobbers EAX, EBX, ECX, EDX)
 cpuid

 ; Move output values to the appropriate registers
 mov esi, eax ; Output EAX
 mov edi, ebx ; Output EBX
 mov ecx, ecx ; Output ECX
 mov edx, edx ; Output EDX

 ; Restore registers and clean up the stack
 pop esi
 pop edi
 pop ebx
 mov esp, ebp
 pop ebp

 ret