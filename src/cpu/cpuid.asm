[bits 32]

global cpuid

cpuid:
  ; Input parameter in EAX register
  mov eax, %edi

  ; Call CPUID instruction (clobbers EAX, EBX, ECX, EDX)
  cpuid

  ; Return values in output registers
  mov %esi, [esp + 4]   ; eax (output)
  mov %edx, [esp + 8]   ; ebx (output)
  mov %ecx, [esp + 12]  ; ecx (output)
  mov %edi, [esp + 16]  ; edx (output)

  ret
