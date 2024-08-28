ENTRY(kernel_main)

PHDRS {
    headers PT_PHDR PHDRS ;
    text_seg PT_LOAD FLAGS(5); /* Read and Execute */
    data_seg PT_LOAD FLAGS(6); /* Read and Write */
}

SECTIONS {

	.kernelheap (0x100000) :  /* Assuming kernel heap starts at 1MB */
    {
      _kernel_heap_start = .;  /* Define symbol at the start of the section */
      *(kernel.o)  /* Include kernel object file in this section */
    }
	
    . = 0xC0000000;  /* Starting virtual address */

    /* Text Section */
    .text : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
    } :text_seg

    . = ALIGN(4096);  /* Align to page boundary */

    /* Data Section */
    .data : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
    } :data_seg

    . = ALIGN(4096);  /* Align to page boundary */

    /* BSS Section */
    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
        *(.bss)
    } :data_seg  /* Use the same segment as .data */
}