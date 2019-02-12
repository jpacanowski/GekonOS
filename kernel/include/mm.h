#ifndef MM_H
#define MM_H

#include <stdint.h>

extern uint32_t end;

uint32_t kmalloc(uint32_t size);
uint32_t kmalloc_int(uint32_t size, uint8_t align, uint32_t *phys);
uint32_t kmalloc_a(uint32_t size);
uint32_t kmalloc_p(uint32_t size, uint32_t *phys);
uint32_t kmalloc_ap(uint32_t size, uint32_t *phys);

#endif
