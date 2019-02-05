#include <isr.h>
#include <video.h>
#include <x86.h>

isr_t interrupt_handlers[256];

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t *regs)
{
    //kprintf("ISR interrupt: %d\n", regs.int_no);
    kprintf(exception_messages[regs->int_no]);
    kprintf(" Exception\n");
}

void irq_handler(registers_t *regs)
{
    //kprintf("IRQ interrupt: %d\n", regs->int_no);

    // After every interrupt we need to send an EOI to the PICs
    // or they will not send another interrupt again
    if(regs->int_no >= 40)
    {
        /* Send reset signal to slave */
        outportb(0xA0, 0x20);
    }

    /* Send reset signal to master */
    outportb(0x20, 0x20);

    if(interrupt_handlers[regs->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}
