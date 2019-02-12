#ifndef COMMON_H
#define COMMON_H

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic_assert(const char* file, uint32_t line, const char* desc);

#endif
