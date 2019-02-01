#ifndef GDT_H
#define GDT_H

#include <types.h>

typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
}
__attribute__((packed)) gdt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
}
__attribute__((packed)) gdt_pointer_t;

void GDT_Init(void);
// void gdt_set_gate(uint8_t num,
//                 uint32_t base, uint32_t limit,
//                 uint8_t access, uint8_t gran);
extern void gdt_flush(u32 gdt_ptr);

#endif
