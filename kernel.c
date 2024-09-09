#include <stdint.h>

void kmain(void)
{
    const uint16_t color = 0x0F00;
    const char *hello = "Hello C world!";
    volatile uint16_t *vga = (volatile uint16_t *)0xb8000;

    for (int i = 0; i < 16; ++i)
    {
        vga[i + 80] = color | (uint16_t)hello[i];
    }
}
