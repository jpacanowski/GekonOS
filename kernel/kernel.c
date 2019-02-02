#include <multiboot.h>
#include <video.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <gui.h>

void kmain(uint32_t magic/*, multiboot_info_t mbi*/)
{
    /* Initialize the video text mode */
    Video_Init();

    /* Check Multiboot bootloader magic number */
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        kpanic("ERROR: Invalid multiboot magic number: %X\n", magic);
    }

    /* Set Global Descriptor Table (GDT) */
    GDT_Init();

    /* Set Interrupt Descriptor Table (IDT) */
    IDT_Init();

    //__asm__ __volatile__("sti");
    //__asm__ __volatile__("int $0x3");
    //__asm__ __volatile__("int $0x4");
    __asm__ __volatile__("sti");

    /* Initialize Task State Segment (TSS) */

    /* Initialize the Programmable Interval Timer (PIT) */
    timer_init(50);

    //__asm__ __volatile__("sti");
    //__asm__ __volatile__("int $0x3");
    //__asm__ __volatile__("int $0x4");

    /* Remap the Programmable Interrupt Controllers (PICs) to new IDT entries */
    
    /* Initialize Memory Manager */
    //kprintf("Detected memory: %d KB", mbi->mem_upper);
    //MM.Init(mbi);
    
    /* Getting alive... Set interrupts */
    //__asm__ __volatile__("sti");
}
