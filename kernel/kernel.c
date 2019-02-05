#include <multiboot.h>
#include <video.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <keyboard.h>
#include <shell.h>
#include <gui.h>

void kmain(uint32_t magic, multiboot_info_t mbi)
{
    UNUSED(magic);
    UNUSED(mbi);

    /* Initialize the video text mode */
    Video_Init();

    /* Set Global Descriptor Table (GDT) */
    GDT_Init();

    /* Set Interrupt Descriptor Table (IDT) */
    IDT_Init();

    /* Initialize Task State Segment (TSS) */

    /* Getting alive... Set interrupts */
    asm volatile("sti");

    /* Initialize the Programmable Interval Timer (PIT) */
    timer_init(50);

    /* Initialize the keyboard */
    keyboard_init();

    /* Shell */
    lsh_loop();

    /* Remap the Programmable Interrupt Controllers (PICs) to new IDT entries */
    
    /* Initialize Memory Manager */
    //kprintf("Detected memory: %d KB", mbi->mem_upper);
    //MM.Init(mbi);
}
