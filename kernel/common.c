#include <video.h>
#include <stdint.h>

void panic_assert(const char* file, uint32_t line, const char* desc)
{
    //cli();
    kprintf("ASSERTION FAILED(");
    kprintf("%s", desc);
    kprintf(") at ");
    kprintf("%s", file);
    kprintf(":");
    kprintf("%d", line);
    kprintf("OPERATING SYSTEM HALTED\n");
    // Halt by going into an infinite loop.
    for(;;);
}