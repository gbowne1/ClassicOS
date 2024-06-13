ENTRY(start)
SECTIONS
{
.text :
    {
        *(.text)
    }

.data :
    {
        *(.data)
    }

.bss :
    {
        *(.bss)
    }

.kernel :
    {
        /* Kernel code */
    . = 0x9000; /* Start of kernel code section */
        *(.kernel)
    }
}