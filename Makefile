CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
AS = nasm

ASFLAGS = -f elf32
CFLAGS = -m32 -c -g -O2 -W -Wall -Wextra -Werror \
	-ffreestanding -std=gnu99 -fno-builtin -Ikernel/include -Ilibc/include
LDFLAGS = -m elf_i386 -T kernel/link.ld -g

OBJS = kentry.o kernel.o video.o gdt.o idt.o pic.o isr.o timer.o x86.o \
	interrupt.o mm.o paging.o kheap.o ordered_array.o vbe.o gui.o keyboard.o \
	shell.o common.o
OBJS_LIBC = math.o stdlib.o string.o ctype.o

all: GekonOS.iso

GekonOS.iso: kernel.bin
	cp kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o $@ iso/

kernel.bin: $(OBJS) $(OBJS_LIBC)
	ld $(LDFLAGS) -o $@ $(OBJS) $(OBJS_LIBC)

%.o: kernel/%.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@

%.o: libc/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o

run:
	qemu-system-i386 -m 32 -cdrom GekonOS.iso
