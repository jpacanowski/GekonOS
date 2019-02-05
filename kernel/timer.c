#include <x86.h>
#include <types.h>
#include <video.h>
#include <isr.h>

uint64_t ticks = 0;

static void timer_callback(registers_t *r)
{
    (void)r;
    ticks++;

    //kprintf("Tick: %d\n", ticks);
}

void timer_init(uint32_t frequency)
{
    // Firstly, register our timer callback
    register_interrupt_handler(IRQ0, timer_callback);

    // The value we send to the PIT is the value to divide
    // its input clock (1193180 Hz) by, to get our required
    // frequency. Important to note is that the divisor
    // must be small enough to fit into 16-bits
    uint32_t divisor = 1193180 / frequency;

    // Divisor has to be sent byte-wise,
    // so split here into upper/lower bytes
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor
    outportb(0x43, 0x36);
    outportb(0x40, low);
    outportb(0x40, high);
}
