.global cpuid
.type cpuid, @function
cpuid:
    push %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    mov %rdi, %rax
    cpuid
    mov %rax, 8(%rbp)
    mov %rbx, 12(%rbp)
    mov %rcx, 16(%rbp)
    mov %rdx, 20(%rbp)
    leave
    ret
