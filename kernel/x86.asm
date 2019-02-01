global outportb
global outportw
global outportl

global inportb
global inportw
global inportl

global gdt_flush
global idt_flush

outportb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret

outportw:
    mov ax, [esp + 8]
    mov dx, [esp + 4]
    out dx, ax
    ret

outportl:
    mov ax, [esp + 8]
    mov dx, [esp + 4]
    out dx, eax
    ret


inportb:
    mov dx, [esp + 4]
    in al, dx
    ret

inportw:
    mov dx, [esp + 4]
    in ax, dx
    ret

inportl:
    mov dx, [esp + 4]
    in eax, dx
    ret


gdt_flush:
    mov eax, [esp + 4]  ; Get the pointer to the GDT
    lgdt [eax]          ; Load the new GDT pointer

    mov ax, 0x10    ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax      ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush ; 0x08 is the offset to our code segment
    .flush:
    ret

idt_flush:
    mov eax, [esp + 4]  ; Get the pointer to the IDT
    lidt [eax]          ; Load the new IDT pointer
    ret
