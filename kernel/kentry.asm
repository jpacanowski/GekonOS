;;
;; the entry point of the kernel
;;

MULTIBOOT_PAGE_ALIGN   equ 1<<0
MULTIBOOT_MEMORY_INFO  equ 1<<1
MULTIBOOT_AOUT_KLUDGE  equ 1<<16
MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM     equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

KERNEL_STACK_SIZE      equ 16384

[BITS 32]
[SECTION .text]

EXTERN kmain

entry:

        ; point esp to the start of the stack (end of memory area)
        mov esp, kernel_stack + KERNEL_STACK_SIZE

        ; push the pointer to the Multiboot information structure
        push ebx

        ; push the magic value (it should be 0x2BADB002)
        push eax

        cli
        call kmain

        ;cli
        ;hlt
        jmp  $

EXTERN code, bss, end

ALIGN 4
mboot:
        dd MULTIBOOT_HEADER_MAGIC
        dd MULTIBOOT_HEADER_FLAGS
        dd MULTIBOOT_CHECKSUM

        dd mboot
        dd code
        dd bss
        dd end
        dd entry

[section .bss]
ALIGN 4                        ; align at 4 bytes
kernel_stack:                  ; label points to beginning of memory
        resb KERNEL_STACK_SIZE ; reserve stack for the kernel