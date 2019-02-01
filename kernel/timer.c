#include <x86.h>
#include <types.h>
#include <video.h>
#include <isr.h>

u32 ticks = 0;

static void timer_callback()
{
    ticks++;

    kprintf("Tick: %d\n", ticks);
}

void timer_init(u32 frequency)
{
    // Firstly, register our timer callback
    register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide
    // its input clock (1193180 Hz) by, to get our required
    // frequency. Important to note is that the divisor
    // must be small enough to fit into 16-bits
    u32 divisor = 1193180 / frequency;

    // Divisor has to be sent byte-wise,
    // so split here into upper/lower bytes
    u8 low = (u8)(divisor & 0xFF);
    u8 high = (u8)((divisor >> 8) & 0xFF);

    // Send the frequency divisor
    outportb(0x43, 0x36);
    outportb(0x40, low);
    outportb(0x40, high);
}
