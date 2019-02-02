#include <gdt.h>
#include <video.h>
#include <string.h>

gdt_entry_t gdt_entries[5];
gdt_pointer_t gdt_ptr;

static void gdt_set_gate(uint8_t num,
                uint32_t base, uint32_t limit,
                uint8_t access, uint8_t gran);

static void gdt_set_gate(uint8_t num,
                uint32_t base, uint32_t limit,
                uint8_t access, uint8_t gran)
{
    /* Base address */
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    /* Limits */
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    /* Granularity */
    gdt_entries[num].granularity |= (gran & 0xF0);

    /* Access flags */
    gdt_entries[num].access = access;
}

void GDT_Init(void)
{
    kprintf("Initializing Global Descriptor Table (GDT)...\n");
    
    memset(&gdt_entries, 0, sizeof(gdt_entry_t)*5);

    gdt_set_gate(0, 0, 0x00000000, 0x00, 0x00); // NULL segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // DPL0 Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // DPL0 Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // DPL3 Code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // DPL3 Data segment

    gdt_ptr.base = (uint32_t)&gdt_entries;
    gdt_ptr.limit = sizeof(gdt_entries) - 1;

    gdt_flush((uint32_t)&gdt_ptr);
}
