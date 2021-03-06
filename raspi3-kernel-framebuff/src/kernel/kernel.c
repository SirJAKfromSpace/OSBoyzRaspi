#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/mem.h>
#include <kernel/atag.h>
#include <kernel/kerio.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>


void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    char buf[256];
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    puts("Initializing Memory Module.....");
    mem_init((atag_t *)atags);
    puts("Memory Module Initializing Complete\n");


    puts("GPU Initializing.....");
    gpu_init();
    puts("GPU Initializing Complete\n");


    puts("Hello, kernel World!\n");

    while (1) {
        gets(buf,256);
        puts(buf);
        putc('\n');
    }
}
