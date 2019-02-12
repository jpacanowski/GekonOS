#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <stdint.h>

typedef void* type_t;
typedef int8_t (*lessthan_predicate_t)(type_t, type_t);

typedef struct
{
    type_t* array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

int8_t standard_lessthan_predicate(type_t a, type_t b);

ordered_array_t create_ordered_array  (uint32_t max_size, lessthan_predicate_t less_than);
ordered_array_t place_ordered_array   (void* addr, uint32_t max_size, lessthan_predicate_t less_than);
void            insert_ordered_array  (type_t item, ordered_array_t* array);
type_t          lookup_ordered_array  (uint32_t i, ordered_array_t* array);
void            remove_ordered_array  (uint32_t i, ordered_array_t* array);
void            destroy_ordered_array (ordered_array_t* array);

extern uint32_t k_malloc(uint32_t size, uint8_t align, uint32_t* phys );
extern void kfree(void* p);

#endif
